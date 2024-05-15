#include "rplidardriver.h"

RPLidarDriver::RPLidarDriver(QObject *parent) : QObject(parent), serialPort(new QSerialPort(this))
{
    // 可以读取则开始读取
    connect(serialPort, &QSerialPort::readyRead, this, &RPLidarDriver::readData);

}
RPLidarDriver::~RPLidarDriver()
{
    closeSerialPort();
}
// 打开串口
void RPLidarDriver::openSerialPort()
{
    serialPort->setPortName("COM4");
    serialPort->setBaudRate(QSerialPort::Baud115200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!serialPort->open(QIODevice::ReadWrite)) {
        qDebug() << "Error: Unable to open port COM3";
    }

}

// 关闭串口
void RPLidarDriver::closeSerialPort()
{
    if (serialPort->isOpen())
        serialPort->close();
}

void RPLidarDriver::readData() {
    static QByteArray buffer;

    buffer.append(serialPort->readAll());
    // 等待至少7字节的描述符
    if (!headerReceived && buffer.size() >= 7) {
        QByteArray descriptor = buffer.mid(0, 7);
        buffer.remove(0, 7);
        headerReceived = true;
        // 处理描述符
    }

    // 持续处理每个5字节的数据响应
    while (buffer.size() >= 5) {
        QByteArray dataResponse = buffer.mid(0, 5);
        buffer.remove(0, 5);
        processResponse(dataResponse);
        emit newDataAvailable();
    }
}

void RPLidarDriver::processResponse(const QByteArray &dataResponse){
    quint8 byte0 = dataResponse[0];
    quint8 byte1 = dataResponse[1];
    quint8 byte2 = dataResponse[2];
    quint8 byte3 = dataResponse[3];
    quint8 byte4 = dataResponse[4];

    bool startFlag = byte0 & 0x01;
    bool inverseStartFlag = byte0 & 0x02;
    quint8 quality = byte0 / 4;

    bool fixedBit = byte1 & 0x01;
    quint8 angleFraction = byte1 >> 1;
    quint8 angleWhole = byte2;

    quint8 distanceWhole = byte3;
    quint8 distanceFraction = byte4;

    double angle = (angleWhole * 128 + angleFraction ) / 64.0;
    double distance = (distanceWhole  + distanceFraction * 256 ) / 4000.0;

    LidarData::lidardata.angle_deg=angle;
    LidarData::lidardata.distance_mm=distance;
    if(distance!=0){
        LidarData::DataContents currData;
        currData.angle_deg=angle;
        currData.distance_mm=distance;
        for(int i = 0; i < NUM_POINTS; i++){
            LidarData::points[i]=LidarData::points[i+1];
        }
        LidarData::points[NUM_POINTS - 1]=currData;
        //qDebug()<<LidarData::points[99].angle_deg<<LidarData::points[99].distance_mm;

    }



    //qDebug()<<LidarData::lidardata.angle_deg<<LidarData::lidardata.distance_mm;
}
void RPLidarDriver::LidarStartScan(){
    // 先发送开始扫描的指令 A5 20
    // 创建指令字节
    QByteArray command;
    command.append(0xA5);
    command.append(0x20);

    headerReceived = false;
    // 发送指令到串口
    if (serialPort->isOpen() && serialPort->isWritable()) {
        serialPort->write(command);
        serialPort->flush();  // 确保命令立即发送
    } else {
        qDebug() << "Serial port is not open or not writable.";
    }

}
void RPLidarDriver::LidarStopScan(){
    // 发送停止扫描的指令
    // 先发送开始扫描的指令 A5 25
    // 创建指令字节
    QByteArray command;
    command.append(0xA5);
    command.append(0x25);

    // 发送指令到串口
    if (serialPort->isOpen() && serialPort->isWritable()) {
        serialPort->write(command);
        serialPort->flush();  // 确保命令立即发送
    } else {
        qDebug() << "Serial port is not open or not writable.";
    }
}

