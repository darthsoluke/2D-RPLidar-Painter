#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}
OpenGLWidget::~OpenGLWidget(){
    makeCurrent();
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
    doneCurrent();
}

void OpenGLWidget::drawShape(Shape shape)
{
    m_shape=shape;

    // Traverse the queue of points
    for (int i = 0; i < NUM_POINTS; i++) {
        pointsCoord[3 * i] = (LidarData::points[i].distance_mm * sin(LidarData::points[i].angle_deg * M_PI / 180)/5);
        pointsCoord[3 * i + 1] = (LidarData::points[i].distance_mm * cos(LidarData::points[i].angle_deg * M_PI / 180)/5);
        pointsCoord[3 * i + 2] = 0;
        //qDebug()<<   pointsCoord[3 * i]<<" "<< pointsCoord[3 * i + 1];
    }
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointsCoord), pointsCoord, GL_STREAM_DRAW);

    repaint();
}

void OpenGLWidget::initializeGL(){
    initializeOpenGLFunctions();
    // 创建VAO,VBO对象并赋予ID

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //绑定VBO和VAO对象
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointsCoord), pointsCoord, GL_STREAM_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, pointsCoord.size() * sizeof(float), pointsCoord.data(), GL_STREAM_DRAW);
    // 告知显卡如何解析缓冲里的属性值
    glVertexAttribPointer(0 , 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
    //开启VAO管理的第一个属性值
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    // 绑定shaderprogram
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/gl.vert");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/gl.frag");
    shaderProgram.link();
}

void OpenGLWidget::paintGL(){
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shaderProgram.bind();
    glBindVertexArray(VAO);
    switch(m_shape){
    case DRAW:
        glPointSize(2.0f);
        glDrawArrays(GL_POINTS, 0, NUM_POINTS);
        break;
    case NONE:
        glClearColor(0.0f,0.0f,0.0f,1.0f);
    default:
        break;

    }

}

void OpenGLWidget::resizeGL(int w, int h){
    glViewport(0, 0, w, h);
}
