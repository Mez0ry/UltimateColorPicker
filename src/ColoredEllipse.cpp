#include "ColoredEllipse.hpp"
#include <QPainter>
#include "Utils.hpp"
#include <QClipboard>
#include <QMouseEvent>

ColoredEllipse::ColoredEllipse(QColor color, int radius, QWidget *parent) : QWidget(parent), m_Color(color), m_Size(radius * 2,radius * 2), m_Radius(radius){

    this->setGeometry(0,0,m_Size.width() ,m_Size.height());
    this->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    this->setMaximumSize(QSize(m_Size.width() + 122,m_Size.height() + 122));
    this->setMinimumSize(m_Size);
}

void ColoredEllipse::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QPoint center(width() / 2, height() / 2);

    painter.setBrush(m_Color);

    painter.setPen(Qt::NoPen);
    painter.drawEllipse(center, m_Radius, m_Radius);

    QFont font(this->font());
    font.setPointSize(Utils::Font::GetAdaptiveFontSize(8));
    painter.setFont(font);

    const QRect rect(center.x() - m_Radius, center.y() - m_Radius, m_Radius*2, m_Radius*2);

    painter.setPen((Utils::CalculateLuminance(m_Color) < 140) ?  Qt::white : Qt::black);
    painter.drawText(rect, Qt::AlignCenter, m_Color.name());
}

void ColoredEllipse::mousePressEvent(QMouseEvent *event){
    Q_UNUSED(event);

    if(event->button() == Qt::MouseButton::LeftButton){
        QGuiApplication::clipboard()->setText(m_Color.name(QColor::HexRgb).toUpper());
    }else if(event->button() == Qt::MouseButton::RightButton){
        emit OnRightClick();
    }
}
