#include "ShadeCircle.hpp"
#include "mainwindow.h"

ShadeCircle::ShadeCircle(QWidget *parent) : QWidget(parent), m_RelativeColor(QColor(55,55,255,255))  , m_IsChoosingColor(false){
    auto size_policy = QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    size_policy.setHeightForWidth(true);
    this->setSizePolicy(size_policy);

    this->setMouseTracking(true);

    m_Margin = 40;

    auto center = this->rect().center();
    quint8 offset = 5;
    QRegion region(center.x() - this->rect().width() / 2 + offset ,center.y() - this->rect().height() / 2 + offset,71,71,QRegion::RegionType::Ellipse);
    this->setMask(region);
    this->setFocusPolicy(Qt::StrongFocus);
}

void ShadeCircle::SetRelativeColor(QColor relative_color)
{
    this->m_RelativeColor = relative_color;
}

void ShadeCircle::SetShadeColor(std::shared_ptr<QColor> shade_color)
{
    if(this->m_SelectedShadeColor){
        this->m_SelectedShadeColor.reset();
    }

    this->m_SelectedShadeColor = shade_color;
    emit ShadeColorChanged();
}

void ShadeCircle::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::LosslessImageRendering);

    painter.setViewport(0, 0 , this->width() , this->height());

    QColor from_color{m_RelativeColor.lighter(200)}, to_color{m_RelativeColor.darker(300)};
    auto shade_grad = QRadialGradient(this->rect().center(),37);

    float pos_offset = 0, // from 0.0f to 1.0f
        dval_factor = 50.f / 10000.f; // = 0.005f

    // fills half of ellipse with lighter color
    for(float lightness_value = 190; lightness_value > 100; lightness_value -= 1){
        shade_grad.setColorAt(pos_offset,m_RelativeColor.lighter(lightness_value));
        pos_offset += dval_factor;
    }

    // fills another half of ellipse with darker color
    for(float darkness_value = 100; darkness_value < 200; darkness_value += 1){
        shade_grad.setColorAt(pos_offset,m_RelativeColor.darker(darkness_value));
        pos_offset += dval_factor;
    }

    painter.setBrush(shade_grad);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(this->rect().center(),37,37);

    auto parent_rect = this->parentWidget()->rect();
    auto parent_center = parent_rect.center();

    auto ring_rad = 5, ring_dia = ring_rad * 2;
    auto hole_rad = 2, hole_dia = hole_rad * 2;

    QPainterPath ring;
    ring.setFillRule(Qt::WindingFill);
    ring.addEllipse(QRect(m_MousePosition.x() - ring_rad, m_MousePosition.y() - ring_rad, ring_dia, ring_dia));

    ring.addEllipse(QRect(m_MousePosition.x() - hole_rad,   m_MousePosition.y() - hole_rad,   hole_dia, hole_dia));

    painter.setPen(Qt::SolidLine);
    painter.setClipPath(ring);
    painter.fillPath(ring, QBrush(Qt::black));
    painter.setClipping(false);
    painter.drawPath(ring);

    QPoint point(parent_center.x() - this->rect().width() / 2 , parent_center.y() - this->rect().height() / 2 );

    this->move(point);
    this->setGeometry(point.x(),point.y(),82,82); // 82,82 best
}

void ShadeCircle::mouseMoveEvent(QMouseEvent *event)
{
    auto event_pos = event->pos();

    if(m_IsChoosingColor){
        m_MousePosition = event_pos;

        *m_SelectedShadeColor = Utils::GetColorFrom(event_pos,this);
        emit ShadeColorChanged();
        emit OnShadeChanged(m_SelectedShadeColor);
    }
}

void ShadeCircle::mousePressEvent(QMouseEvent *event)
{
    auto event_pos = event->pos();

    m_MousePosition = event_pos;

    *m_SelectedShadeColor = Utils::GetColorFrom(event_pos,this);
    emit ShadeColorChanged();

    m_IsChoosingColor = true;
}

void ShadeCircle::mouseReleaseEvent(QMouseEvent *event)
{
    auto event_pos = event->pos();

    *m_SelectedShadeColor = Utils::GetColorFrom(event_pos,this);
    emit ShadeColorChanged();

    m_IsChoosingColor = false;
}
