#include "ball.h"
#include <cmath> // 添加包含

Ball::Ball(QObject *parent) : QObject(parent), m_random(QRandomGenerator::global()->generate()) {
    m_direction = QPointF(m_random.generateDouble() * 2.0 - 1.0, 
                        m_random.generateDouble() * 2.0 - 1.0);
    m_direction /= qSqrt(m_direction.x() * m_direction.x() + m_direction.y() * m_direction.y());
}

QPointF Ball::position() const {
    return m_position;
}

void Ball::setPosition(const QPointF &newPosition) {
    if (m_position != newPosition) {
        m_position = newPosition;
        // 添加边界检查确保不会超出
        ensureInBounds(QRect(0, 0, 10000, 10000)); // 使用足够大的边界
        emit positionChanged();
    }
}

QColor Ball::color() const {
    return m_color;
}

void Ball::setColor(const QColor &newColor) {
    if (m_color != newColor) {
        m_color = newColor;
        emit positionChanged();
    }
}

int Ball::radius() const {
    return m_radius;
}

void Ball::setRadius(int newRadius) {
    if (m_radius != newRadius) {
        m_radius = newRadius;
        emit positionChanged();
    }
}

float Ball::speed() const {
    return m_speed;
}

void Ball::setSpeed(float newSpeed) {
    if (m_speed != newSpeed) {
        m_speed = newSpeed;
        m_currentSpeed = m_speed;
    }
}

bool Ball::isVisible() const {
    return m_visible;
}

void Ball::setVisible(bool newVisible) {
    if (m_visible != newVisible) {
        m_visible = newVisible;
        emit positionChanged();
    }
}

void Ball::move(const QRect &bounds) {
    if (!m_visible) return;

    // 更新速度变化
    if (m_speedVariation > 0) {
        float variation = m_speedVariation * 2.0 * (m_random.generateDouble() - 0.5);
        m_currentSpeed = m_speed * (1.0f + variation);
    }

    QPointF newPos = m_position + m_direction * m_currentSpeed;
    
    // 边界碰撞检测 - 修正后的逻辑
    bool bounce = false;
    if (newPos.x() < bounds.left()) {
        newPos.setX(bounds.left());
        m_direction.setX(-m_direction.x());
        bounce = true;
    } else if (newPos.x() > bounds.right()) {
        newPos.setX(bounds.right());
        m_direction.setX(-m_direction.x());
        bounce = true;
    }
    
    if (newPos.y() < bounds.top()) {
        newPos.setY(bounds.top());
        m_direction.setY(-m_direction.y());
        bounce = true;
    } else if (newPos.y() > bounds.bottom()) {
        newPos.setY(bounds.bottom());
        m_direction.setY(-m_direction.y());
        bounce = true;
    }
    
    setPosition(newPos);
}

void Ball::randomMove(const QRect &bounds) {
    m_visibleCounter -= 16;
    
    if (m_visibleCounter <= 0) {
        m_visible = !m_visible;
        if (m_visible) {
            // 确保新位置在边界内
            setPosition(QPointF(
                bounds.left() + m_radius + m_random.generateDouble() * (bounds.width() - 2 * m_radius),
                bounds.top() + m_radius + m_random.generateDouble() * (bounds.height() - 2 * m_radius)
            ));
            m_visibleCounter = m_invisibleDuration;
        } else {
            m_visibleCounter = m_random.bounded(500, 2000);
        }
    }
}

void Ball::updateDirection() {
    m_direction = QPointF(m_random.generateDouble() * 2.0 - 1.0, 
                        m_random.generateDouble() * 2.0 - 1.0);
    m_direction /= qSqrt(m_direction.x() * m_direction.x() + m_direction.y() * m_direction.y());
  
    // 速度变化幅度
    m_speedVariation = 0.3f + m_random.generateDouble() * 0.4f;
}

void Ball::ensureInBounds(const QRect &bounds) {
    QPointF pos = m_position;
    bool changed = false;
    
    if (pos.x() < bounds.left()) {
        pos.setX(bounds.left());
        changed = true;
    } else if (pos.x() > bounds.right()) {
        pos.setX(bounds.right());
        changed = true;
    }
    
    if (pos.y() < bounds.top()) {
        pos.setY(bounds.top());
        changed = true;
    } else if (pos.y() > bounds.bottom()) {
        pos.setY(bounds.bottom());
        changed = true;
    }
    
    if (changed) {
        setPosition(pos);
    }
}