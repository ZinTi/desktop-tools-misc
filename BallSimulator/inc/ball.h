#ifndef BALL_H
#define BALL_H

#include <QObject>
#include <QColor>
#include <QPointF>
#include <QRandomGenerator>
#include <QRect> // 添加
#include <QPointF> // 添加

class Ball : public QObject {
    Q_OBJECT
public:
    explicit Ball(QObject *parent = nullptr);

    QPointF position() const;
    void setPosition(const QPointF &newPosition);
    QColor color() const;
    void setColor(const QColor &newColor);
    int radius() const;
    void setRadius(int newRadius);
    float speed() const;
    void setSpeed(float newSpeed);
    bool isVisible() const;
    void setVisible(bool newVisible);
    void move(const QRect &bounds);
    void randomMove(const QRect &bounds);
    void updateDirection();

private:
    void ensureInBounds(const QRect &bounds);

signals:
    void positionChanged();

private:
    QPointF m_position;
    QColor m_color = Qt::red;
    int m_radius = 20;
    float m_speed = 1.0f;
    bool m_visible = true;
    QPointF m_direction = {1.0f, 1.0f};
    float m_speedVariation = 0.0f;
    float m_currentSpeed = 1.0f;
    int m_visibleCounter = 0;
    int m_invisibleDuration = 2000;
    QRandomGenerator m_random;
};

#endif // BALL_H

