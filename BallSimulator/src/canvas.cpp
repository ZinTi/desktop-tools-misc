#include "canvas.h"
#include <QPainter>
#include <QResizeEvent>
#include <QDebug>

Canvas::Canvas(QWidget *parent) : QWidget(parent) {
    setMinimumSize(400, 400);
    m_timer.setInterval(16); // ~60 FPS
    connect(&m_timer, &QTimer::timeout, this, &Canvas::animate);
    m_timer.start();
}

void Canvas::setBall(Ball *ball) {
    m_ball = ball;
    
    // 初始化小球位置在窗口中心
    if (m_ball) {
        m_ball->setPosition(rect().center());
    }
    
    connect(m_ball, &Ball::positionChanged, this, qOverload<>(&Canvas::update));
}

void Canvas::setGridVisible(bool visible) {
    m_gridVisible = visible;
    update();
}

void Canvas::setGridColor(const QColor &color) {
    m_gridColor = color;
    update();
}

void Canvas::setBackgroundColor(const QColor &color) {
    m_backgroundColor = color;
    update();
}

void Canvas::setRandomMode(bool randomMode) {
    m_randomMode = randomMode;
}

void Canvas::setInvisibleDuration(int duration) {
    if (m_ball) m_ball->setProperty("invisibleDuration", duration);
}

void Canvas::setBallSize(int size) {
    if (m_ball) m_ball->setRadius(size);
}

void Canvas::setBallColor(const QColor &color) {
    if (m_ball) m_ball->setColor(color);
}

void Canvas::setBallSpeed(float speed) {
    if (m_ball) m_ball->setSpeed(speed);
}

void Canvas::setVariableSpeed(bool variable) {
    m_variableSpeed = variable;
    if (m_ball && m_variableSpeed) {
        m_ball->updateDirection();
    }
}

void Canvas::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 绘制背景
    painter.fillRect(rect(), m_backgroundColor);
    
    // 绘制网格
    if (m_gridVisible) {
        drawGrid(painter);
    }
    
    // 绘制小球
    if (m_ball && m_ball->isVisible()) {
        drawBall(painter);
    }
}

void Canvas::resizeEvent(QResizeEvent *event) {
    // 更新小球位置到新中心
    if (m_ball) {
        m_ball->setPosition(rect().center());
    }
    QWidget::resizeEvent(event);
}

void Canvas::animate() {
    if (!m_ball) return;
    
    // 使用整个窗口的边界，减去小球半径确保不会超出
    QRect adjustedRect = rect().adjusted(
        m_ball->radius(), 
        m_ball->radius(), 
        -m_ball->radius(), 
        -m_ball->radius()
    );
    
    if (m_randomMode) {
        m_ball->randomMove(adjustedRect);
    } else {
        m_ball->move(adjustedRect);
        if (m_variableSpeed && QRandomGenerator::global()->bounded(100) < 5) {
            m_ball->updateDirection();
        }
    }
}

void Canvas::drawGrid(QPainter &painter) {
    painter.save();
    painter.setPen(QPen(m_gridColor, 1, Qt::DotLine));
    
    // 绘制坐标轴
    QPoint center = rect().center();
    painter.drawLine(rect().left(), center.y(), rect().right(), center.y());
    painter.drawLine(center.x(), rect().top(), center.x(), rect().bottom());
    
    // 绘制刻度
    painter.setPen(QPen(m_gridColor, 1, Qt::SolidLine));
    const int step = 50;
    
    // X轴刻度
    for (int x = center.x() % step; x < rect().width(); x += step) {
        painter.drawLine(x, center.y() - 5, x, center.y() + 5);
    }
    
    // Y轴刻度
    for (int y = center.y() % step; y < rect().height(); y += step) {
        painter.drawLine(center.x() - 5, y, center.x() + 5, y);
    }
    
    // 绘制象限标签
    painter.setPen(Qt::darkGray);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);
    
    painter.drawText(center.x() + 10, 20, "I");
    painter.drawText(center.x() - 20, 20, "II");
    painter.drawText(center.x() - 30, rect().height() - 10, "III");
    painter.drawText(center.x() + 10, rect().height() - 10, "IV");
    
    painter.restore();
}

void Canvas::drawBall(QPainter &painter) {
    painter.save();
    painter.setBrush(m_ball->color());
    painter.setPen(Qt::NoPen);
    
    // 直接使用小球的绝对位置，不再转换坐标系
    painter.drawEllipse(m_ball->position(), m_ball->radius(), m_ball->radius());
    
    painter.restore();
}

