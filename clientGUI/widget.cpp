#include "widget.h"
#include "ui_widget.h"

BarController::BarController()
{
    lossRate = pro = 0.0f;
    sharedMemSig = false;
    FileSize = 0;
    return;
}
BarController::~BarController()
{
    return;
}
Transmission::Transmission()
{
    return;
}
Transmission::~Transmission()
{
    return;
}
void BarController::run()
{
    lossRate = pro = 0.0f;
    emit(updateBar(0.0, 0, lossRate));
    if (!sharedMemSig)
    {
        //发射内存读取失败信号
        emit(updateBar(0, pro,lossRate));
        return;
    }
    while (exitSig==false)
    {
        lossRate = lpBase->lossRate;
        if ((mode == _PUSH)&&(FileSize == 0 || lpBase->over))
        {
            pro = 1.0; 
            emit(updateBar(lpBase->tSize, (char)(pro * 100), lossRate));
            break;
        }
        else if ((mode == _PULL) && (lpBase->over))
        {
            pro = 1.0;
            emit(updateBar(lpBase->tSize, (char)(pro * 100), lossRate));
            break;
        }
        else 
            pro = (float) lpBase->tSize/ (float)FileSize;
        if (lpBase->tSize >= FileSize && mode !=_PULL)
            break;
        if (mode == _PULL && pro >= 1.0 && (!lpBase->over))
            pro = 0.99;
        emit(updateBar(lpBase->tSize, (char)(pro*100), lossRate));
        
        //qDebug() << "progress: " << lpBase->tSize << "loss " << lossRate << endl;
        Sleep(50);
    }
    free(lpBase);
    return;
}
void Transmission::run()
{
    QString err_str;
    bool suc;
    emit(updateState(true, false, err_code, err_str));
    stimer = QDateTime::currentDateTime();
    if (mode == _PUSH)
        suc = tftp_client::sendFile(sock, server_addr, std_file, fileType, 0, timeout, tryCNT, err_code);
    else suc = tftp_client::recvFile(sock,server_addr,std_file,BIN_FILE_TYPE,0,timeout,tryCNT,recvPath,err_code);
    closesocket(sock);
    emit(updateState(false, suc,  err_code, err_str));
}
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    tranState=false;
    modelSwitcher = new SwitchControl(this);
    modelSwitcher->move(100, 315);
    modelSwitcher->setBackgroundColor(QColor(255, 255, 255));
    modelSwitcher->setMinimumSize(85, 45);
    modelSwitcher->setMaximumSize(85, 45);
    modelSwitcher->setToggled(true);
    ui->progressBar->setRange(0, 100);
    ui->progressBar->reset();
    ui->folderButton->setIconSize(QSize(60,60));
    QIcon folderButtonIcon(QCoreApplication::applicationDirPath()+"./image/folder.png");
    ui->folderButton->setIcon(folderButtonIcon);

    connect(modelSwitcher, SIGNAL(chgModelLabel(bool)), this, SLOT(changeModelLabel(bool)));//选择模式
    connect(&barCon, SIGNAL(updateBar(unsigned long, char, float)), this, SLOT(chgProcessBar(unsigned long, char,float)));
    connect(&trans, SIGNAL(updateState(bool ,bool,int, QString)), this, SLOT(chgTranState(bool ,bool, int, QString)));
}

Widget::~Widget()
{
    delete ui;
}
bool file_dir_isvalid(QString &str,int opt)
{
    if (str.isEmpty())return false;
    if (opt == _PULL)
    {
        QDir dir(str);
        if (!dir.exists())return false;
    }
    else
    {
        QFile file(str);
        if (!file.exists())return false;
    }
    return true;
}
void Widget::on_folderButton_clicked()
{
    if (tranState)
   {
       QMessageBox::warning(NULL, "警告", "存在未完成的传输");
       return;
   }
   if (opt== _PULL)
   {
       QString dir = QFileDialog::getExistingDirectory(this, "目录", QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
       if (dir.isEmpty())
       {
           QMessageBox::warning(NULL, "警告", "目录不能为空");
           return;
       }
       ui->folderLine->setText(dir);
   }
   else
   {
       QString filePath = QFileDialog::getOpenFileName(this, "文件", QDir::homePath(), "all (*.*)");
       if (filePath.isEmpty())
       {
           QMessageBox::warning(NULL, "警告", "文件名不能为空");
           return;
       }
       ui->folderLine->setText(filePath);
   }
    return;
}
void Widget::errHandler(int err_code, QString &err_msg)
{
    //由Qt处理
    if (err_code == ERR_DEST_IP)
        QMessageBox::warning(NULL, "警告", "IP地址错误");
    else if (err_code == ERR_QT_WFILE)
        QMessageBox::warning(NULL, "警告", "文件不存在或打开失败");
    else if (err_code == ERR_QT_WFOLDER)
        QMessageBox::warning(NULL, "警告", "目录不存在");
    else if (err_code == ERR_QT_WREMOTEFILE)
        QMessageBox::warning(NULL, "警告", "远端文件名不存在");
    //展示传输错误
    else if (err_code == ERR_UNKNOWN)
        err_msg = "未知错误";
    else if (err_code == ERR_UNKNOWN_FILE)
        err_msg = "未知文件";
    else if (err_code == ERR_ACCESS_VIOLATION)
        err_msg = "拒绝访问";
    else if (err_code == ERR_DISK_FULL)
        err_msg = "磁盘已满";
    else if (err_code == ERR_ILLOP)
        err_msg = "非法操作";
    else if (err_code == ERR_UNKNOWN_TID)
        err_msg = "未知TID";
    else if (err_code == ERR_FILE_EXIST)
        err_msg = "请求的文件不存在";
    else if (err_code == ERR_UNKNOWN_USER)
        err_msg = "未知用户";
    else if (err_code == ERR_SENDTO)
        err_msg = "发送报文异常";
    else if (err_code == ERR_BUFF)
        err_msg = "缓冲区异常";
    else if (err_code == ERR_NO_ANS)
        err_msg = "服务器无响应";
    else if (err_code == ERR_READFILE_SHORT)
        err_msg = "读取文件长度小于预期";
    else if (err_code == ERR_TIMEOUT)
        err_msg = "响应超时";
    else if (err_code == ERR_WRTIEFILE_SHORT)
        err_msg = "写入文件长度小于预期";
    else if (err_code == ERR_CFILEF)
        err_msg = "打开文件异常";
    else
        err_msg = QString::fromUtf8("系统或未知错误 ")+ QString::number(err_code);
    return;
}
void Widget::chgProcessBar(unsigned long tSize, char pro, float lossRate)
{
    ui->progressBar->setValue(pro);
    ui->progressBar->update();
    QDateTime cur = QDateTime::currentDateTime();
    qint64 msec = trans.stimer.msecsTo(cur);
    float speed = ((float)tSize/1024) / ((float)msec / 1000);
    ui->speedLabel->setText(QString::number(speed,'f', 2)+"Kb/s");
    ui->lossRateLabel->setText("丢包率" + QString::number(lossRate, 'f', 2) + "%");
    return;
}
void Widget::chgTranState(bool running ,bool suc, int err_code, QString err_str)
{
    if(running)
    { 
        tranState = true;
        return;
    }
    tranState = false;
    barCon.FileSize = 0;
    QTreeWidgetItemIterator it(ui->messageTreeWidget);
    while (*it)
    {
        int childn = (*it)->childCount();
        QTreeWidgetItemIterator iter(*it);
        for (int i = 0; i <= childn; i++)iter++;
        if (!(*iter))break;
        else
            for (int i = 0; i <= childn; i++)it++;
    }
    if (suc) 
    {
        (*it)->setData(2, 0, "成功");
        (*it)->setTextColor(0, QColor(68, 201, 176, 255));
        (*it)->setTextColor(1, QColor(68, 201, 176, 255));
        (*it)->setTextColor(2, QColor(68, 201, 176, 255));
        //qDebug() << "succeed to transmission" << endl;
    }
    else
    {
        errHandler(err_code, err_str);
        (*it)->setData(2, 0, "失败");
        (*it)->setTextColor(0, QColor(0xc9, 0x37, 0x56, 255));
        (*it)->setTextColor(1, QColor(0xc9, 0x37, 0x56, 255));
        (*it)->setTextColor(2, QColor(0xc9, 0x37, 0x56, 255));
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setData(0, 0, "错误说明: "+err_str);
        (*it)->addChild(item);
        barCon.exitSig=true;
        //qDebug() << "failed to transmission" << " " << "err: " << err_code << endl;
    }
    return;
}
void Widget::addLog(QString ip, bool opt, QString filePath)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    //qDebug() << "warn: " << warn << endl;
    //item->setTextColor(0, QColor(219, 93, 107, 255));
    if (opt == _PUSH)
        item->setText(0, "上传");
    else item->setText(0, "下载");
    item->setText(1,filePath);
    item->setText(2, "传输中");
    item->setTextColor(0, QColor(0, 47, 167, 255));//blue
    
    item->setTextColor(1, QColor(0, 47, 167, 255));//blue
    item->setTextColor(2, QColor(0, 47, 167, 255));//blue


    QTreeWidgetItem* item2 = new QTreeWidgetItem();
    item2->setData(0, 0, "ip: " + ip);
    QTreeWidgetItem* item3 = new QTreeWidgetItem();
    item3->setData(0, 0, "port: "+ ui->portLine->text());
    item->addChild(item2);
    item->addChild(item3);
    ui->messageTreeWidget->addTopLevelItem(item);
    ui->messageTreeWidget->scrollToBottom();
}
unsigned long Widget::checkFile(QString &file)
{
    if(!file_dir_isvalid(file,opt))return 0;
    std::string std_file = std::string(file.toLocal8Bit());
    HANDLE hFile = (HANDLE)CreateFileA(std_file.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return 0;
    DWORD fileSize = GetFileSize(hFile, NULL);
    CloseHandle(hFile);
    return fileSize;
}

void Widget::on_commitButton_clicked()
{
    QString err_str;
    //准备ip,port,timeout,tryCNT设置
    std::string ip_str = std::string(ui->IPLine->text().toLocal8Bit());
    USHORT port = ui->portLine->text().toUShort();
    trans.timeout = ui->timeoutLine->text().toLong(); 
    trans.tryCNT = ui->tryCNTLine->text().toInt();
    //初始化socket
    bool suc = tftp_client::socketinit(trans.err_code);
    if (!suc)
    {
        err_str = "套接字初始化失败";
        errHandler(trans.err_code, err_str);
        return;
    }
    //建立socket 在tran线程内部回收
    suc = tftp_client::createSocket(ip_str, port, trans.server_addr, trans.sock, trans.timeout, trans.err_code);
    if (!suc) 
    {
        err_str = "创建套接字异常";
        errHandler(trans.err_code, err_str);
        return;
    }
    //建立传输状态通信全局变量 在barCon线程内部回收
    barCon.lpBase = (tftp_client::tranState*)(std::malloc(sizeof(tftp_client::tranState)));
    barCon.structSize = sizeof(tftp_client::tranState);
    //连接全局变量通信
    barCon.sharedMemSig = tsinit(barCon.lpBase);
    if (!barCon.sharedMemSig)
    {
        err_code = ERR_QT_WSHAREDM;
        errHandler(err_code, err_str);
        return;
    }
    if (opt == _PUSH)//上传操作处理
    {
        QString filePath = ui->folderLine->text();
        trans.std_file = std::string(filePath.toLocal8Bit());
        barCon.FileSize = checkFile(filePath);
        if (barCon.FileSize == 0)
        {
            err_str = "文件不存在或打开失败";
            errHandler(ERR_QT_WFILE, err_str);
            return;
        }
        trans.fileType = tftp_client::getTypeByPath(trans.std_file);
        addLog(ui->IPLine->text(),opt,filePath);
        barCon.exitSig = false;
        barCon.start();
        trans.start();
    }
    else
    {
        //提取保存路径
        QString fileDir = ui->folderLine->text();
        if (!file_dir_isvalid(fileDir, opt))
        {
            err_str = "文件夹不存在";
            errHandler(ERR_QT_WFOLDER, err_str);
            return;
        }
        trans.recvPath = std::string(fileDir.toLocal8Bit());
        if (trans.recvPath.at(trans.recvPath.length() - 1) != '\\' && trans.recvPath.at(trans.recvPath.length() - 1) != '/')
            trans.recvPath.append("/");
        //提取请求文件名
        QString RemoteFileName = ui->remoteFileLine->text();
        if (RemoteFileName.isEmpty())
        {
            errHandler(ERR_QT_WREMOTEFILE, err_str);
            return;
        }
        trans.std_file = std::string(RemoteFileName.toLocal8Bit());
        std::string all = (trans.recvPath + trans.std_file);
        addLog(ui->IPLine->text(), opt, QString::fromLocal8Bit(all.c_str()));
        barCon.FileSize = 33554432;
        barCon.exitSig = false;
        barCon.start();
        trans.start();
    }
}
void Widget::changeModelLabel(bool model)
{
    if (model)
    {
        ui->modelLabel->setText("下载");
        ui->remoteLabel->setHidden(false);
        ui->remoteFileLine->setHidden(false);
        trans.mode = barCon.mode = opt = _PULL;
    }
    else
    {
        ui->remoteLabel->setHidden(true);
        ui->remoteFileLine->setHidden(true);
        ui->modelLabel->setText("上传");
        trans.mode = barCon.mode = opt = _PUSH;
    }
}
SwitchControl::SwitchControl(QWidget *parent)
    :QWidget(parent),
      m_nHeight(38),
      m_bChecked(true),
      m_radius(19.0),
      m_nMargin(4),
      m_checkedColor(QColor(0, 120, 199)),
      m_thumbColor(QColor(0,102,120)),
      m_disabledColor(QColor(242,242,242)),
      m_background(QColor(242,242,242))
{
    setCursor(Qt::PointingHandCursor);
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(onTimeout()));
}

void SwitchControl::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    QColor background;
    QColor thumbColor;
    qreal dOpacity;
    if(isEnabled()){    //可用状态
        if(m_bChecked){    //打开状态
            background = m_background;
            thumbColor = m_checkedColor;
            dOpacity = 0.600;
        }else{      //关闭状态
            background = m_background;
            thumbColor = m_thumbColor;
            dOpacity = 0.800;
        }
    }else{  //不可用状态
        background = m_background;
        dOpacity = 0.260;
        thumbColor = m_disabledColor;
    }

    painter.setRenderHint(QPainter::Antialiasing);

    //绘制大椭圆
    painter.setPen(QPen(QColor(169, 169, 169), 2, Qt::SolidLine, Qt::RoundCap));
    painter.setBrush(background);
    painter.setOpacity(dOpacity);
    path.addRoundedRect(QRectF(m_nMargin, m_nMargin, width() - 2 * m_nMargin, height() - 2 * m_nMargin), m_radius, m_radius);
    painter.drawPath(path.simplified());
    //painter

    //绘制小椭圆
    painter.setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap));
    painter.setBrush(thumbColor);
    painter.setOpacity(1.0);
    painter.drawEllipse(QRectF(m_nX - (m_nHeight / 5), m_nY - (m_nHeight / 3.3), height() / 1.5, height() / 1.5));
}

//鼠标按下事件
void SwitchControl::mousePressEvent(QMouseEvent *event)
{
    if(isEnabled()){
        if(event -> buttons() & Qt::LeftButton){
            event -> accept();
        }else{
            event -> ignore();
        }
    }
}

//鼠标释放事件-切换开关状态、发射toggled()信号
void SwitchControl::mouseReleaseEvent(QMouseEvent *event)
{
    if(isEnabled()){
        if((event->type() == QMouseEvent::MouseButtonRelease) && (event->button() == Qt::LeftButton)){
            event->accept();
            m_bChecked = !m_bChecked;
            emit toggled(m_bChecked);
            m_timer.start(6);
        }else{
            event->ignore();
        }
    }
}

//大小改变事件
void SwitchControl::resizeEvent(QResizeEvent *event)
{
    m_nX = m_nHeight / 2;
    m_nY = m_nHeight / 2;
    QWidget::resizeEvent(event);
}

//默认大小
QSize SwitchControl::sizeHint() const
{
    return minimumSizeHint();
}

//最小大小
QSize SwitchControl::minimumSizeHint() const
{
    return QSize(2 * (m_nHeight + m_nMargin),m_nHeight + 2 * m_nMargin);
}

//切换状态-滑动
void SwitchControl::onTimeout()
{
    if(m_bChecked){
        m_nX += 1;
        if(m_nX >= width() - m_nHeight)
            m_timer.stop();
    }else{
        m_nX -= 1;
        if(m_nX <= m_nHeight / 2)
            m_timer.stop();
    }
    update();
    emit chgModelLabel(m_bChecked);
}

//返回开关状态-打开：true 关闭：false
bool SwitchControl::isToggled() const
{
    return m_bChecked;
}

//设置开关状态
void SwitchControl::setToggled(bool checked)
{
    m_bChecked = checked;
    m_timer.start(6);
}

//设置背景颜色
void SwitchControl::setBackgroundColor(QColor color)
{
    m_background = color;
}

//设置选中颜色
void SwitchControl::setCheckedColor(QColor color)
{
    m_checkedColor = color;
}

//设置不可用颜色
void SwitchControl::setDisabledColor(QColor color)
{
    m_disabledColor = color;
}

//void Widget::on_cancelButton_clicked()
//{
//    if(!tranState)
//    {
//        QMessageBox::information(NULL,"警告","没有传输中的任务");
//        return;
//    }
//    if(barCon.isRunning())
//    {
//        barCon.FileSize=0;
//        while(barCon.isRunning())Sleep(50);
//    }
//    if(trans.isRunning())
//    {
//        trans.exit();
//        while(trans.isRunning())Sleep(50);
//    }
//  
//    QMessageBox::information(NULL,"警告", "已终止");
//    return;
//}
