#include "EllipseButton.hpp"
#include <QPainter>

EllipseButton::EllipseButton(QWidget *parent, QSize size) : QPushButton(parent), m_Size(size), m_Radius(m_Size.height() / 2) {
    this->setGeometry(0, 0, m_Size.width(), m_Size.height());
}

void EllipseButton::SetFillLevel(qreal value)
{
    if (m_FillLevel != value) {
        m_FillLevel = value;
        update();
        emit FillLevelChanged(value);
    }
}

void EllipseButton::StartAnimation(qreal targetLevel, int duration_ms)
{
    if (m_Animation.state() == QAbstractAnimation::Running)
        m_Animation.stop();

    m_Animation.setTargetObject(this);
    m_Animation.setPropertyName("FillLevel");
    m_Animation.setStartValue(GetFillLevel());
    m_Animation.setEndValue(targetLevel);
    m_Animation.setDuration(duration_ms);
    m_Animation.start();
}

void EllipseButton::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rect = this->rect();

    QRadialGradient grad(rect.center(), m_Radius);

    m_FillLevel = qBound(0.0,m_FillLevel,1.0);

    grad.setColorAt(0.0, m_ToFillColor);
    grad.setColorAt(m_FillLevel, m_FromFillColor);
    painter.setPen(Qt::NoPen);
    painter.setBrush(grad);
    painter.drawEllipse(rect.center(), m_Radius, m_Radius);
}
