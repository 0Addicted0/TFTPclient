#ifndef WIDGET_H
#define WIDGET_H
#pragma execution_character_set("utf-8")
#include "client_def.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWidget>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <QDateTime>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

#include <QWidget>
#include <QTimer>
class SwitchControl : public QWidget
{
    Q_OBJECT

public:
    explicit SwitchControl(QWidget* parent = 0);

    //返回开关状态-打开：true 关闭：false
    bool isToggled() const;

    //设置开关状态
    void setToggled(bool checked);

    //设置背景颜色
    void setBackgroundColor(QColor color);

    //设置选中颜色
    void setCheckedColor(QColor color);

    //设置不可用颜色
    void setDisabledColor(QColor color);

protected:
    //绘制开关
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

    //鼠标按下事件
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    //鼠标释放事件-切换开关状态、发射toggled()信号
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    //大小改变事件
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

    //缺省大小
    QSize sizeHint() const Q_DECL_OVERRIDE;
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;

signals:
    //状态改变时，发射信号
    void toggled(bool checked);
    void chgModelLabel(bool);

private slots:
    //状态切换时，用于产生滑动效果
    void onTimeout();

private:
    bool m_bChecked;            //是否选中
    QColor m_background;        //背景颜色
    QColor m_checkedColor;      //选中颜色
    QColor m_disabledColor;     //不可用颜色
    QColor m_thumbColor;        //拇指颜色
    qreal m_radius;             //圆角
    qreal m_nX;                 //x点坐标
    qreal m_nY;                 //y点坐标
    qint16 m_nHeight;           //高度
    qint16 m_nMargin;           //外边距
    QTimer m_timer;             //定时器
};
class BarController :public QThread
{
    Q_OBJECT
public:
    BarController();
    ~BarController();
    unsigned long  FileSize;
    size_t structSize;
    tftp_client::tranState *lpBase;
    bool sharedMemSig;
    int mode;
    bool exitSig=false;
signals:
    void updateBar(unsigned long tSize, char p, float lossRate);
private:
    float pro,lossRate;
protected:
    void run() override;
};

class Transmission :public QThread
{
    Q_OBJECT
public:
    Transmission();
    ~Transmission();
    SOCKET sock;
    sockaddr_in server_addr;
    int err_code,fileType;
    std::string std_file,recvPath;
    long timeout;
    int tryCNT;
    int mode;
    QDateTime stimer;
signals:
    void updateState(bool running,bool suc, int err_code,QString err_str);
protected:
    void run() override;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();
    SwitchControl *modelSwitcher;
    void errHandler(int err_code, QString &err_msg);
    unsigned long checkFile(QString& file);
private slots:
    void on_folderButton_clicked();
    void on_commitButton_clicked();
    void changeModelLabel(bool model);
    void chgProcessBar(unsigned long tSize, char pro, float lossRate);
    void chgTranState(bool running, bool suc, int err_code,QString err_str);
    //void on_cancelButton_clicked();
private:
    Ui::Widget* ui;
    bool opt = _PULL;
    bool tranState;
    int err_code;
    BarController barCon;
    Transmission trans;
    void addLog(QString ip, bool opt, QString filePath);
};
#endif // WIDGET_H
