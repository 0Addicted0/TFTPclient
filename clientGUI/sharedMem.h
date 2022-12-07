#ifndef __SHARED_MEM__H_
#define __SHARED_MEM__H_
#include<stdio.h>
#include<stdint.h>
#include<windows.h>
#define SHARED_SEM_NAME "tftpClient_s_sem"        //写入信号量
#define SHARED_MEM_NAME "tftpClient_s_mem"        //共享内存
#define SHARED_LOCKSEM_NAME "tftpClient_s_lcoksem"//阻塞锁信号量
namespace tftp_client
{
    namespace sharedM
    {
        struct tranState
        {
            unsigned long tSize = 0;
            double lossRate= 0.0;
            bool over = false;
        };

        inline bool CreateSharedMem(size_t size, HANDLE& hSemaphore, HANDLE& hMapFile, LPVOID& lpBase)
        {
            hSemaphore = CreateSemaphoreA(NULL, 0, 1, SHARED_SEM_NAME);
            if (!hSemaphore)return false;
            hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, SHARED_MEM_NAME);
            if (!hMapFile)return false;
            lpBase = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
            if (!lpBase)return false;
            return true;
        }
        inline void CloseSharedMem(HANDLE& hSemaphore, HANDLE& hMapFile, LPVOID& lpBase)
        {
            UnmapViewOfFile(lpBase);
            CloseHandle(hSemaphore);
            CloseHandle(hMapFile);
        }
        inline void CloseLocker(HANDLE& hSemaphore)
        {
            CloseHandle(hSemaphore);
        }
        inline bool CreateLocker(HANDLE& hSemaphore)
        {
            hSemaphore = CreateSemaphoreA(NULL, 0, 1, SHARED_LOCKSEM_NAME);
            if (!hSemaphore)return false;
            return true;
        }
    }//sharedM
}//tftp_client
#endif //__SHARED_MEM__
