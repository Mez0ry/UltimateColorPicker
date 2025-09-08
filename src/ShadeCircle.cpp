#include "ShadeCircle.hpp"
#include <QPainterPath>
#include "Utils.hpp"

ShadeCircle::ShadeCircle(QWidget *parent) : QWidget(parent){
    auto size_policy = QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    size_policy.setHeightForWidth(true);
    this->setSizePolicy(size_policy);

    this->setMouseTracking(true);

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

    const qreal dpr = devicePixelRatioF();

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    painter.setViewport(0, 0 , this->width() , this->height());

    auto shade_grad = QRadialGradient(this->rect().center(),m_Radius);

    auto idx = 0;

    // fills half of ellipse with lighter color
    for(int lightness_value = 190; lightness_value > 100; lightness_value -= 1, ++idx){
        double normalize_pos = (double) idx / (double) (m_ShadesSteps - 1);
        shade_grad.setColorAt(normalize_pos,m_RelativeColor.lighter(lightness_value));
    }

    // fills another half of ellipse with darker color
    for(int darkness_value = 100; darkness_value < 390; darkness_value += 1, ++idx){
        double normalize_pos = (double) idx / (double) (m_ShadesSteps - 1);

        shade_grad.setColorAt(normalize_pos,m_RelativeColor.darker(darkness_value));
    }

    QPainterPath shades_grad_path;
    shades_grad_path.setFillRule(Qt::WindingFill);
    shades_grad_path.addEllipse(this->rect().center(), m_Radius , m_Radius);

    painter.setPen(Qt::SolidLine);
    painter.setClipPath(shades_grad_path);
    painter.fillPath(shades_grad_path, QBrush(shade_grad));
    painter.setClipping(true);
    painter.drawPath(shades_grad_path);

    auto parent_rect = this->parentWidget()->rect();
    auto parent_center = parent_rect.center();

    auto ring_rad = 8 * dpr,
         ring_dia = (ring_rad * 2) * dpr;

    auto hole_rad = 5 * dpr,
        hole_dia = (hole_rad * 2) * dpr;

    QPainterPath ring;
    ring.setFillRule(Qt::WindingFill);
    ring.addEllipse(QRect(m_MousePosition.x() - ring_rad, m_MousePosition.y() - ring_rad, ring_dia, ring_dia));

    ring.addEllipse(QRect(m_MousePosition.x() - hole_rad, m_MousePosition.y() - hole_rad, hole_dia, hole_dia));

    painter.setPen(Qt::SolidLine);
    painter.setClipPath(ring);
    painter.fillPath(ring, shade_grad);
    painter.setClipping(true);
    painter.drawPath(ring);

    const auto dia = m_Radius * 2;

    QPoint point(parent_center.x() - this->rect().width() / 2 , parent_center.y() - this->rect().height() / 2 );

    this->move(point);
    this->setGeometry(point.x(),point.y(),dia + 4,dia + 4);

    QRegion region(this->rect().center().x() - this->rect().width() / 2, this->rect().center().y() - this->rect().height() / 2 ,dia + 4, dia + 4,QRegion::RegionType::Ellipse);
    this->setMask(region);
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
