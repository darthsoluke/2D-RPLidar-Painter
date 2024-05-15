#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H
#include "LidarData.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>

class OpenGLWidget : public QOpenGLWidget , QOpenGLFunctions_4_5_Core
{
    Q_OBJECT;
public:
    enum Shape{NONE, DRAW};
    explicit OpenGLWidget(QWidget *parent=nullptr);
    ~OpenGLWidget();
protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;
signals:

public slots:
    void drawShape(Shape shape);


private:
    unsigned int VBO,VAO;
    QOpenGLShaderProgram shaderProgram;
    // 枚举量
    Shape m_shape;
    float pointsCoord[3 * NUM_POINTS]={0};
    QMatrix4x4 projection;
    QMatrix4x4 view;
    QMatrix4x4 model;
    void setupMatrices();
};

#endif // OPENGLWIDGET_H
