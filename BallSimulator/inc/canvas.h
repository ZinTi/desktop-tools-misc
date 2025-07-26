#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QTimer>
#include "ball.h"
#include <QPainter> // 添加
#include <QResizeEvent> // 添加

class Canvas : public QWidget {
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);
    void setBall(Ball *ball);
    void setGridVisible(bool visible);
    void setGridColor(const QColor &color);
    void setBackgroundColor(const QColor &color);
    void setRandomMode(bool randomMode);
    void setInvisibleDuration(int duration);
    void setBallSize(int size);
    void setBallColor(const QColor &color);
    void setBallSpeed(float speed);
    void setVariableSpeed(bool variable);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void animate();

private:
    void drawGrid(QPainter &painter);
    void drawBall(QPainter &painter);

    Ball *m_ball = nullptr;
    QTimer m_timer;
    bool m_gridVisible = true;
    QColor m_gridColor = QColor(200, 200, 200, 150);
    QColor m_backgroundColor = Qt::white;
    bool m_randomMode = false;
    bool m_variableSpeed = false;
    QRect m_canvasRect;
};

#endif // CANVAS_H

