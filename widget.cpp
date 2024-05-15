#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    rplidardriver=new RPLidarDriver();
    connect(rplidardriver, &RPLidarDriver::newDataAvailable, this, [this]{
        if(LidarData::lidardata.distance_mm!=0)
        ui->distance->setText(QString::number(LidarData::lidardata.distance_mm));
        ui->angle->setText(QString::number(LidarData::lidardata.angle_deg));
        ui->figureOutput->drawShape(OpenGLWidget::DRAW);
    });


}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_startButton_clicked()
{
    rplidardriver->openSerialPort();
    rplidardriver->LidarStartScan();
    ui->figureOutput->drawShape(OpenGLWidget::DRAW);
}


void Widget::on_stopButton_clicked()
{
    rplidardriver->LidarStopScan();
    ui->figureOutput->drawShape(OpenGLWidget::NONE);

}

