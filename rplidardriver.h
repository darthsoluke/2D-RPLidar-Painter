#ifndef RPLIDARDRIVER_H
#define RPLIDARDRIVER_H

#include <QTimer>
#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <LidarData.h>
/*
 * 先打开串口
 * 如果按下按钮就发送指令
 * 如果串口可读取数据就读取数据
 */
class RPLidarDriver : public QObject
{
    Q_OBJECT
public:
    explicit RPLidarDriver(QObject *parent = nullptr);
    ~RPLidarDriver();
    void openSerialPort();       // 打开串口
    void closeSerialPort();      // 关闭串口
signals:
    void newDataAvailable();  // 新数据可用信号
    void isNewScan();
public slots:
    void LidarStartScan();      // 开始扫描的槽函数
    void LidarStopScan();       // 停止扫描的槽函数
private slots:
    void readData();             // 从串口读取数据
private:
    QSerialPort *serialPort;
    bool headerReceived;
    void processResponse(const QByteArray &dataResponse);
};

#endif // RPLIDARDRIVER_H
