#include "client_def.h"
#include <cstdlib>
#include <ctime>
#include <qdebug.h>
#define STR_TYPE_NUM 3
namespace tftp_client 
{
    tranState* pkt;
    char stdTextType[4][4] = { "txt", "csv", "doc"};
    extern char logPath[30] = "./tftp_clien_log.txt";
    std::string currentTimetoStr()
    {
        char tmp[64];
        SYSTEMTIME st = { 0 };
        GetLocalTime(&st);
        WORD year = st.wYear;
        WORD month = st.wMonth;
        WORD date = st.wDay;
        WORD hh = st.wHour;
        WORD mm = st.wMinute;
        WORD ss = st.wSecond;
        WORD ms = st.wMilliseconds;
        sprintf(tmp, "%04u-%02u-%02u-%02u:%02u:%02u(%03u)", year, month, date, hh, mm, ss, ms);
        return std::string(tmp);
    }
    void logWriter(int opt, std::string filep,USHORT blk)
    {
        FILE* file = fopen(logPath,"a+");
        std::string opt_str;
        time_t now = time(nullptr);
        char* curr_time = ctime(&now);
        fprintf(file, "@%s  ", currentTimetoStr().c_str());
        //fprintf(file, "@%s  ", curr_time);

        if (opt == _PULL)opt_str = "下载";
        else if (opt == _PUSH) opt_str = "上传";
        else opt_str = "重试";
        
        fprintf(file, "[%s] : " ,opt_str.c_str());
        if (opt == _RETRY)fprintf(file, "%s [%d]\n", filep.c_str(), blk);
        else fprintf(file, "%s\n", filep.c_str());
        fclose(file);
        return;
    }
    void path2Name(const char* path, char* name)
    {
        //qDebug();
        const char* tpath = path;
        int len = min(strlen(path), MAX_PATH);
        tpath = path + len - 1;
        len--;
        while ((*tpath != '/') && (*tpath) != '\\' && len)tpath--, len--;
        if (len)tpath++;
        while (*tpath)
        {
            *name = *tpath;
            name++, tpath++;
        }
        return;
    }
    int getTypeByPath(std::string &filePath)
    {
        int type;
        char name[MAX_PATH] = { 0 };
        path2Name(filePath.c_str(), name);
        int len = strlen(name);
        char* p = name + len - 1;
        while (p<=name)
        {
            if (*p == '.')
            {
                p--;
                break;
            }
        }
        bool isText = false;
        for (int i = 0; i < STR_TYPE_NUM; i++)
        {
            if (strcmp(p, stdTextType[i]) == 0)
            {
                isText = true;
                break;
            }
        }
        if (isText)return STR_FILE_TYPE;
        return BIN_FILE_TYPE;
    }

    bool isIPAddressValid(const char* pszIPAddr)
    {
        if (!pszIPAddr)
            return false;
        char IP1[100] = {0}, cIP[4] = {0};
        int len = strlen(pszIPAddr);
        int i = 0, j = len - 1;
        int k, m = 0, n = 0, num = 0;
        //去除首尾空格(取出从i-1到j+1之间的字符):
        while (pszIPAddr[i++] == ' ');
        while (pszIPAddr[j--] == ' ');

        for (k = i - 1; k <= j + 1; k++)
        {
            IP1[m++] = *(pszIPAddr + k);
        }
        IP1[m] = '\0';
        char* p = IP1;
        while (*p != '\0')
        {
            if (*p == ' ' || *p < '0' || *p>'9') return false;
            cIP[n++] = *p; //保存每个子段的第一个字符，用于之后判断该子段是否为0开头
            int sum = 0;  //sum为每一子段的数值，应在0到255之间
            while (*p != '.' && *p != '\0')
            {
                if (*p == ' ' || *p < '0' || *p>'9') return false;
                sum = sum * 10 + *p - 48;  //每一子段字符串转化为整数
                p++;
            }
            if (*p == '.') {
                if ((*(p - 1) >= '0' && *(p - 1) <= '9') && (*(p + 1) >= '0' && *(p + 1) <= '9'))//判断"."前后是否有数字，若无，则为无效IP，如“1.1.127.”
                    num++;  //记录“.”出现的次数，不能大于3
                else
                    return false;
            };
            if ((sum > 255) || (sum > 0 && cIP[0] == '0') || num > 3) return false;//若子段的值>255或为0开头的非0子段或“.”的数目>3，则为无效IP
            if (*p != '\0') p++;
            n = 0;
        }
        if (num != 3)
            return false;
        return true;
    }
    bool tsinit(tranState* buff)
    {
        if (!buff)return false;
        pkt = buff;
        pkt->over = false;
        pkt->tSize = 0;
        pkt->lossRate = 0.0f;

        return true;
    }
    bool socketinit(int &err_code)
    {
        WSADATA wsaData;
        int nRC = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (nRC)
        {
            err_code = WSAGetLastError();
            return false;
        }
        return true;
    }
    bool createSocket(std::string sip, USHORT sport, sockaddr_in& saddr, SOCKET& csock, long timeout,int& err_code)
    {
        if (!isIPAddressValid(sip.c_str()))
        {
            err_code = ERR_DEST_IP;
            return false;
        }
        csock = socket(AF_INET, SOCK_DGRAM, 0);
        if (csock == SOCKET_ERROR)
        {
            err_code = WSAGetLastError();
            return false;
        }
        memset(&saddr, 0, sizeof(sockaddr_in));

        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(sport);
        saddr.sin_addr.s_addr = inet_addr(sip.c_str());

        return true;
    }
    bool build_data_header(char* buff_sd, USHORT blkNum,int &err_code)
    {
        if (!buff_sd)
        {
            err_code = ERR_BUFF;
            return false;
        }
        *((USHORT*)buff_sd) = htons(_DATA);
        *((USHORT*)(buff_sd+2)) = htons(blkNum);
        return true;
    }
    bool build_data_pkt(HANDLE hFile,char *buff_sd ,int tryCNT, DWORD toReadlen, DWORD &nb, USHORT blkNum,int &err_code)
    {
        memset(buff_sd, 0, BUFF_SIZE);
        if (!build_data_header(buff_sd, blkNum, err_code))
            return false;
        bool rd_suc = false;
        for (int i = 1; i <= tryCNT && (!rd_suc); i++)
        {
            rd_suc = ReadFile(hFile, buff_sd + _DATA_HEADER_LEN, toReadlen, &nb, NULL);
            if (!rd_suc)
            {
                err_code = GetLastError();
                rd_suc = false;
            }
            if (nb != toReadlen)
            {
                err_code = ERR_READFILE_SHORT;
                rd_suc = false;
            }
        }
        if (!toReadlen)nb = 1;
        return rd_suc;
    }
    bool buildReqPkt(std::string& fileName, int fileType, char* buff_sd, int &len, int model,int& err_code)
    {
        if (fileType<STR_FILE_TYPE ||fileType > BIN_FILE_TYPE)
        {
            err_code = ERR_FILETYPE;
            return false;
        }
        if (!buff_sd)
        {
            err_code = ERR_BUFF;
            return false;
        }
        memset(buff_sd, 0, BUFF_SIZE);
        int mod_len = ((fileType == STR_FILE_TYPE) ? (strlen(STR_FILE)) : (strlen(BIN_FILE))); // 加 '\0'
        int file_len = fileName.length();
        int opcode_len = 2;
        len = opcode_len + (file_len + 1) + (mod_len + 1);
        if (model)
            *((USHORT*)buff_sd) = htons(_WRQ);
        else
            *((USHORT*)buff_sd) = htons(_RRQ);
        memcpy(buff_sd + 1 + 1, fileName.c_str(), fileName.length());
        buff_sd[1 + fileName.length() + 1] = _ENDL;
        if (fileType == STR_FILE_TYPE)  memcpy(buff_sd + 1 + (fileName.length() + 1) + 1, STR_FILE, mod_len);
        else  memcpy(buff_sd + 1 + (fileName.length() + 1) + 1, BIN_FILE, mod_len);
        buff_sd[1 + fileName.length() + 1 + mod_len + 1] = _ENDL;
        return true;
    }
    bool gener_send(SOCKET csock, sockaddr_in& saddr, char* buff_sd, int len, int tryCNT, int& err_code)
    {
        int tlen = 0;
        for (int i = 1; i <= tryCNT; i++)
        {
            tlen = sendto(csock, buff_sd, len, 0, (sockaddr*)(&saddr), sizeof(saddr));
            if (tlen != len)
            {
                err_code = ERR_SENDTO;
                return false;
            }
            else break;
        }
        return true;
    }
    bool gener_recv(SOCKET& csock, sockaddr_in& saddr, sockaddr_in& recv_saddr, bool isfirst, long timeout, char* buff_rv, int &recvLen,int& err_code)
    {
        /*
            不带有重试计数
            通用报文接受函数
            1. 对报文进行字节转换处理:opcode blkNum转为主机字节大小端
            2. 不对报文进行语义处理
        */
        if (!csock)
        {
            err_code = ERR_SOCK;
            return false;
        }
        if (!buff_rv)
        {
            err_code = ERR_BUFF;
            return false;
        }
        memset(buff_rv, 0, BUFF_SIZE);
        bool getans = false, err_flag = false;
        int recv_saddr_length = sizeof(recv_saddr);

        FD_SET readFds;
        TIMEVAL tv = { timeout/1000, timeout % 1000 *1000};// 设置超时时间

        FD_ZERO(&readFds);// 初始化文件描述符
        FD_SET(csock, &readFds);
        int selectRet = select(csock + 1, &readFds, NULL, NULL, &tv);// 咋超时时间内监听socket

        if (selectRet == SOCKET_ERROR)// 若返回错误
        {
            err_code = WSAGetLastError();// 记录错误号
            FD_CLR(csock, &readFds);  //清理文件描述符
            return false;
        }
        else if (selectRet == 0) //在超时时间内socket无变化
        {
            err_code = ERR_NO_ANS;// 记录超时
            FD_CLR(csock, &readFds);// 回收文件描述符
            return false;
        }
        else// socket产生变化 接收消息
        {
            recvLen = recvfrom(csock, buff_rv, BUFF_SIZE, 0, (sockaddr*)(&recv_saddr), &recv_saddr_length);
            if (recvLen > 0)
            {
                *((USHORT*)(buff_rv)) = ntohs(*((USHORT*)(buff_rv))); // 大小端转换
                if (*((USHORT*)(buff_rv)) != 1 && *((USHORT*)(buff_rv)) != 2)
                    *((USHORT*)(buff_rv + 2)) = ntohs(*((USHORT*)(buff_rv + 2)));
                if (!isfirst)//如果不是第一次通信
                {
                    if (memcmp((char*)(&saddr), (char*)(&recv_saddr), sizeof(recv_saddr_length)) == 0)
                        getans = true;
                }
                else//第一次通信 记录server使用的port
                {
                    saddr = recv_saddr;
                    getans = true;
                }
            }
            else
            {
                err_flag = true;
                err_code = WSAGetLastError();
            }
        }
        if (!getans && (!err_flag))
            err_code = ERR_NO_ANS;
        return getans;
    }
    bool err_handler(char *buff_rv, int&err_code)
    {
        /*
            返回值:
                true  返回的报文是opcode = ERR 或存在其他错误且已经成功处理
                false 返回报文不是ERR
                err_code 为报文返回的错误值,如果错误码为ERR_UNKNOWN还需要解析buff_rv中的字符串(因此catch到错误以后需要立即返回,不能修改接收缓冲区)
        */
        if (!buff_rv)
        {
            err_code = ERR_BUFF;
            return true;
        }
        if (*((USHORT*)(buff_rv)) == _ERROR)
        {
            err_code = (int)(*((USHORT*)(buff_rv + 2)));//取得错误号
            switch (err_code)
            {
                case 0:err_code = ERR_UNKNOWN; break;
                case 1:err_code = ERR_UNKNOWN_FILE; break;
                case 2:err_code = ERR_ACCESS_VIOLATION; break;
                case 3:err_code = ERR_DISK_FULL; break;
                case 4:err_code = ERR_ILLOP; break;
                case 5:err_code = ERR_UNKNOWN_TID; break;
                case 6:err_code = ERR_FILE_EXIST; break;
                case 7:err_code = ERR_UNKNOWN_USER; break;
                default:
                    err_code;// = ERR_UNKNOWN;
            };
            return true;
        }
        return false;
    }
    bool ack_recv(SOCKET &csock, sockaddr_in &saddr, long timeout, int &counter,int tryCNT, char *buff_rv,USHORT blkNum, bool isfirst, int &recvLen ,int &err_code)
    {   
        /*
            用于接收确认号 会解析recv buff并进行错误处理
            带有重试计数器
        */
        sockaddr_in  t_saddr;
        if(counter!=-1)counter = 0;
    retry:
        if (gener_recv(csock, saddr, t_saddr, isfirst, timeout, buff_rv, recvLen,err_code))
        {

            if (*((USHORT*)(buff_rv)) == _ACK && *((USHORT*)(buff_rv + 2)) == blkNum) return true;
            else
            {
//#ifdef DEBUG
//                std::cout << "opcode: " << *((USHORT*)(buff_rv)) << " " << "blk: " << *((USHORT*)(buff_rv + 2)) << std::endl;
//#endif // DEBUG
                //这里需要 处理opcode = 5 的负载
                if (err_handler(buff_rv, err_code))//错误处理完成
                    return false;
                err_code = ERR_MSG;
            judge:
                auto et = clock();
                if (counter <= tryCNT && counter!=-1)
                {
                    counter++;
                    goto retry;
                }
                else return false;
            }
        }
        else
        {
            err_code = ERR_TIMEOUT;
            goto judge;
        }
        return false;
    }
    bool makeReq(SOCKET& csock, sockaddr_in& saddr, long timeout, int tryCNT, char* buff_rv, char* buff_sd, int len, int model,int& err_code)
    {
        //model 1 表示为发送写请求 需要等待接收ack=0
        if (!gener_send(csock, saddr, buff_sd, len, tryCNT, err_code))
            return false;
        int recvLen;
        int cnter = 0;
        if (model)
            return ack_recv(csock, saddr, timeout, cnter, tryCNT, buff_rv, 0, true, recvLen ,err_code);
        else return true;
    }
    void update(unsigned long BlkNum,float lossRate)
    {
        if (lossRate >= 1.0)
        {
            lossRate = 1.0;
        }
        pkt->lossRate = lossRate*100.0;
        pkt->tSize = BlkNum * 512;
        //qDebug() << "loss: "<< lossRate << endl;
        return;
    }
    void emitover()
    {
        pkt->over= true;
    }
    bool send_data(SOCKET& csock, sockaddr_in& saddr, std::string filePath, char *buff_sd, char *buff_rv, int model, long timeout,int tryCNT,int&err_code)
    {
        if (!csock)
        {
            err_code = ERR_SOCK;
            return false;
        }
        if (!buff_sd)
        {
            err_code = ERR_BUFF;
            return false;
        }
        HANDLE hFile = (HANDLE)CreateFileA(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
        if (!hFile)
        {
            err_code = ERR_CFILEF;
            return false;
        }
        DWORD fileSize = GetFileSize(hFile, NULL);
        bool suc = true;
        USHORT blkNum = 1; unsigned long sucCNT = 1;
        int cnter = -1;//用于记录ack_recv标记
        if (!model)
        {
            int cnt = (fileSize / 512) ;
            
            DWORD nb;
            int recvLen=0,counter = 0;
            float total = 0.0,losspkt=0.0;
            while(cnt--)
            {
                suc = false;
                counter = 0;
                if (!build_data_pkt(hFile, buff_sd, tryCNT, 512, nb, blkNum,err_code))
                    break;
                retry:
                    if(!gener_send(csock,saddr, buff_sd,(int)nb + _DATA_HEADER_LEN, tryCNT,err_code))
                        break;
                    if (!ack_recv(csock, saddr, timeout, cnter, tryCNT, buff_rv, blkNum, false, recvLen, err_code))
                    {
                        //if (err_code == ERR_TIMEOUT && counter < tryCNT)
                        if (counter < tryCNT)
                        {
#ifdef DEBUG
                            std::cout << "losed pkt: " << blkNum << "(retry)" << std::endl;
#endif // DEBUG
                            logWriter(_RETRY, filePath, blkNum);
                            losspkt++;
                            total++;
                            counter++;
                            update(total, losspkt / total);
                            goto retry;
                        }
                        else break;
                    }
                total++;
                blkNum++;
                sucCNT++;
                update(sucCNT, losspkt/total);
                suc = true;
            }
            update(total, losspkt / total);
            DWORD res = (fileSize % 512);
            if (suc)
            {
                suc = false;
                if (!build_data_pkt(hFile, buff_sd, tryCNT, res, nb, blkNum, err_code))
                    goto ret;
                rretry:
                    if (!gener_send(csock, saddr, buff_sd, (int)nb + _DATA_HEADER_LEN, tryCNT,err_code))
                        goto ret;
                    if (!ack_recv(csock, saddr, timeout, cnter, tryCNT, buff_rv, blkNum,false, recvLen,err_code))
                    {
                        if (err_code == ERR_TIMEOUT && counter < tryCNT)
                        {
#ifdef DEBUG
                            std::cout << "losed pkt: " << blkNum << "(retry)" << std::endl;
#endif // DEBUG
                            logWriter(_RETRY, filePath, blkNum);
                            losspkt++;
                            total++;
                            counter++;
                            update(total, losspkt / total);
                            goto rretry;
                        }
                        else goto ret;
                    }
                total++;
                blkNum++;
                sucCNT++;
                update(sucCNT, losspkt / total);
                suc = true;
                emitover();
            }
        }
    ret:
        CloseHandle(hFile);
        return suc;
    }
    void clearFileBuff(RVFILEBUFF *buff)
    {
        if (!buff)return;
        for(int i=0;i<=MAXBLK*MAXRD;i++)
            if ((*buff).blk[i])
                free((*buff).blk[i]);
        free(buff);
        return;
    }
    bool checkBLKintergrity(RVFILEBUFF& fileBuffer, unsigned long tail)
    {
        for (unsigned long i = 1; i <= tail; i++)
            if (!fileBuffer.blk[i]) return false;
        return true;
    }
    bool make_ack(SOCKET csock, sockaddr_in saddr, char *buff_sd,USHORT blkNum, int tryCNT, int &err_code)
    {
        if (!buff_sd)
        {
            err_code = ERR_BUFF;
            return false;
        }
        memset(buff_sd, 0, BUFF_SIZE);
        *((USHORT *)(buff_sd)) = htons(_ACK);
        *((USHORT*)(buff_sd + 2)) = htons(blkNum);
        int sendLen = _DATA_HEADER_LEN + 1; // '\0'
        return gener_send(csock,saddr,buff_sd, sendLen, tryCNT,err_code);
    }
    bool writeFile(std::string& recvPath,RVFILEBUFF &buff,unsigned long blks, int lastLen, int &err_code)
    {
        HANDLE hFile = CreateFileA(recvPath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
        if (!hFile)
        {
            err_code = ERR_CFILEF;
            return false;
        }
        DWORD cbData = 0;
        DWORD len = 512;
        for (unsigned long i = 1; i <= blks && i<=(MAXBLK)*MAXRD; i++)
        {
            if (i == blks)
               len = lastLen;
            if (!WriteFile(hFile, buff.blk[i], len, &cbData, NULL))
                goto err_ret;
            if (len != cbData)
            {
                err_code = ERR_WRTIEFILE_SHORT;
                goto err_ret;
            }
        }
        if (!FlushFileBuffers(hFile))
            goto err_ret;
        CloseHandle(hFile);
        return true;
    err_ret:
        CloseHandle(hFile);
        err_code = GetLastError();
        return false;
    }
    bool recv_data(SOCKET csock, sockaddr_in &saddr, char *buff_sd, char *buff_rv,int model, long timeout, int tryCNT, std::string &fileRecvPath, int &err_code)
    {
        RVFILEBUFF* pfileBuffer = (RVFILEBUFF *)malloc(sizeof(RVFILEBUFF));
        if (!pfileBuffer)
        {
            err_code = ERR_BUFF;
            return false;
        }
        memset(pfileBuffer, 0, sizeof(RVFILEBUFF));
        if (!csock)
        {
            err_code = ERR_SOCK;
            return false;
        }
        if (!buff_sd)
        {
            err_code = ERR_BUFF;
            return false;
        }
        sockaddr_in t_saddr;
        int recvLen, lastLen=-1;
        int blocking = 1;
        bool isFirst = true;


        unsigned long total = 0,round=0,loss = 0;
        float lossrate = 0.0f;
        int timeoutCNT = 0;
        USHORT tblk = 0;
        while (blocking)
        {
            retry:
                recvLen = 0;
                if (!gener_recv(csock, saddr, t_saddr, isFirst, timeout, buff_rv, recvLen, err_code))
                {
                    if (timeoutCNT <= tryCNT)
                    {
                        if (!make_ack(csock, saddr, buff_sd, tblk, tryCNT, err_code))// 重发上一次的blk num
                        {
                            clearFileBuff((pfileBuffer));
                            return false;
                        }
                        loss++;
                        timeoutCNT++;
                        update(total, (float)loss / (float)total);
                        logWriter(_RETRY, fileRecvPath, tblk);
                        goto retry;
                    }
                    clearFileBuff(pfileBuffer);// 回收内存
                    return false;
                }
            tblk = *((USHORT*)(buff_rv + 2));
            total = round * (MAXBLK)+tblk + round;
            timeoutCNT = 0;
            if (err_handler(buff_rv, err_code))
            {
                clearFileBuff((pfileBuffer));
                return false;
            }
            if (!( * pfileBuffer).blk[total])//新段
            {
                if (isFirst)isFirst = false;
                (*pfileBuffer).blk[total] = (char*)malloc(static_cast<size_t>(recvLen) - _DATA_HEADER_LEN + 1);
                if (!(*pfileBuffer).blk[total])
                {
                    err_code = ERR_BUFF;
                    clearFileBuff((pfileBuffer));
                    return false;
                }
                memset((*pfileBuffer).blk[total], 0, static_cast<size_t>(recvLen) - _DATA_HEADER_LEN + 1);
                memcpy((*pfileBuffer).blk[total], (buff_rv + _DATA_HEADER_LEN), static_cast<size_t>(recvLen) - _DATA_HEADER_LEN);
                if (recvLen < (512 + _DATA_HEADER_LEN))//末尾段
                {
                    if (lastLen == -1)
                        lastLen = recvLen - _DATA_HEADER_LEN;
                    if (checkBLKintergrity((*pfileBuffer), total))//所有段接收完毕
                    {
                        blocking = 0;
                        if (!writeFile(fileRecvPath, (*pfileBuffer), total, lastLen, err_code))
                            return false;
                        emitover();
                    }
                    //else
                    //    blocking = 2;
                }
                else if (tblk == MAXBLK)
                {
                    round++;
                    qDebug() << round << endl;
                }
            }
            update(total, (float)loss / (float)total);
            if (!make_ack(csock, saddr, buff_sd, tblk, tryCNT,err_code))
            {
                clearFileBuff((pfileBuffer));
                return false;
            }
            //if (!blocking)//最后多等一会
            //{
                //int t_err_code = err_code;
                //gener_recv(csock,saddr,t_saddr,false,1,buff_rv,recvLen,err_code);
                //if (err_code != ERR_NO_ANS)//主机未发出信息
                    //return true;
                //if(recvLen)
                //    err_handler(buff_rv,err_code);//否则尝试错误处理
                //return false;
            //}
        }
        return true;
    }
    bool sendFile(SOCKET& csock, sockaddr_in& saddr, std::string& filePath, int fileType, int model, long timeout, int tryCNT, int &err_code)
    {
        tsinit(pkt);
        err_code = 0;
        if (!csock)
        {
            err_code = ERR_SOCK;
            return false;
        }
        char buff_sd[BUFF_SIZE];
        char buff_rv[BUFF_SIZE];
        int len;
        char fileNamec[MAX_PATH]={0};
        path2Name(filePath.c_str(), fileNamec);
        std::string fileName = fileNamec;
        logWriter(_PUSH, filePath, 0);

        if (!buildReqPkt(fileName, fileType, buff_sd, len, 1,err_code))//model 1 -> write req
            return false;

        if (!makeReq(csock, saddr, timeout, tryCNT, buff_rv, buff_sd, len, 1,err_code))
            return false;

        //发送数据段
        if (!send_data(csock, saddr, filePath, buff_sd, buff_rv, model, timeout, tryCNT, err_code))
        {
            logWriter(_PUSH, filePath + "(失败)\n", 0);
            return false;
        }
        logWriter(_PUSH, filePath + "(成功)\n", 0);
        return true;
    }
    bool recvFile(SOCKET& csock, sockaddr_in& saddr, std::string& fileName, int fileType, int model, long timeout, int tryCNT, std::string &fileRecvPath,int& err_code)
    {
        //pkt->over = false;
        tsinit(pkt);
        err_code = 0;
        if (!csock)
        {
            err_code = ERR_SOCK;
            return false;
        }
        char buff_sd[BUFF_SIZE];
        char buff_rv[BUFF_SIZE];
        int len;

        std::string RecvPath = fileRecvPath;
        char tname[MAX_PATH] = { 0 };
        path2Name(fileName.c_str(), tname);
        RecvPath.append(tname);
        logWriter(_PULL, fileName, 0);

        if (!buildReqPkt(fileName, fileType, buff_sd, len, 0, err_code))
            return false;

        if (!makeReq(csock, saddr, timeout, tryCNT, buff_rv, buff_sd, len, 0,err_code))
            return false;

        //接收数据段
        if (!recv_data(csock, saddr, buff_sd, buff_rv, model, timeout, tryCNT, RecvPath, err_code))
        {
            logWriter(_PULL, fileName + "(失败)\n", 0);
            return false;
        }
        logWriter(_PULL, fileName + "(成功)\n", 0);
        return true;
    }
}
