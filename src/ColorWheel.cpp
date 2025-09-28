#include "ColorWheel.hpp"
#include <QApplication>
#include <QClipboard>
#include <QColorSpace>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QRegularExpression>
#include <QScreen>
#include <QStaticText>
#include <QTransform>
#include <math.h>
#include <qmath.h>

#include "QColorExtended.hpp"
#include "Utils.hpp"
#include "ColorCombo.hpp"
#include "ColorInfo.hpp"

ColorWheel::ColorWheel(QWidget *parent, QColor selected_color, quint16 margin) : QWidget(parent), m_ShadeCircle(this){
    this->setMouseTracking(false);
    m_SelectedColor = std::make_shared<QColor>(selected_color);

    this->m_ShadeCircle.SetShadeColor(std::make_shared<QColor>(*m_SelectedColor));

    this->m_Margin = margin;
    this->setGeometry(0, 0, m_Size.width(),m_Size.height());

    std::call_once(m_InitOnceFlag,[&](){
        ColorInfo::GetInstance()->SetColorContext(m_ShadeCircle.GetSelectedShade());
        ColorInfo::GetInstance()->setParent(this);
        ColorInfo::GetInstance()->Setup();
    });

    connect(ColorInfo::GetInstance(),&ColorInfo::InfoChanged, this, [=](){
        ColorInfo::GetInstance()->SetupColorInfo();
        this->update();
    });

    connect(&m_ShadeCircle, &ShadeCircle::ShadeChanged, this, [=]( std::shared_ptr<QColor> new_color){
        ColorInfo::GetInstance()->SetColorContext(new_color);
        ColorInfo::GetInstance()->SetupColorInfo();

        ColorCombo::GetInstance()->SetBaseColor(new_color.get());
        m_ShadeCircle.update();
        this->update();
    });

    connect(this, &ColorWheel::ColorChanged, this, [=](){
        ColorInfo::GetInstance()->SetColorContext(m_SelectedColor);
        ColorInfo::GetInstance()->SetupColorInfo();
        ColorCombo::GetInstance()->SetBaseColor(m_SelectedColor.get());
        this->update();
    });

    OnColorChange();
}

void ColorWheel::paintEvent(QPaintEvent * event){
    const auto center = QPointF(this->width() / 2, this->height() / 2);
    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing);
    painter.setViewport(m_Margin, m_Margin, this->width() - 2 * m_Margin , this->height() - 2 * m_Margin);

    QTransform transform;
    transform.translate(width() / 2.0, height() / 2.0);
    transform.scale(m_ScaleFactor, m_ScaleFactor);
    transform.translate(-width() / 2.0, -height() / 2.0);

    painter.setTransform(transform);

    OnPaintEvent(painter);

    auto val_grad = QRadialGradient(center, 16);
    val_grad.setColorAt(0.0f, *m_ShadeCircle.GetSelectedShade());
    val_grad.setColorAt(1.f, *m_ShadeCircle.GetSelectedShade());

    painter.setBrush(val_grad);
    painter.drawEllipse(this->rect());

    painter.setPen(Qt::black);
    painter.setBrush(*m_SelectedColor.get());

    const double angle = (qAtan2(m_LastCursorPos.x() - (m_Radius), m_LastCursorPos.y() - (m_Radius)) + M_PI * 1.5);
    const QPoint final_ellipse_pos(center.x() + m_Radius * std::cos(-angle ), center.y() + m_Radius * std::sin(-angle ));

    const int radius = 7;
    const auto inversed_color = Utils::GetInveseredColorIf(*m_SelectedColor,[&](int luminance){ return (luminance < 30 );});
    painter.setPen(inversed_color);

    painter.setBrush(*m_SelectedColor);
    painter.drawEllipse(final_ellipse_pos, radius, radius);

    this->m_ShadeCircle.SetRelativeColor(*m_SelectedColor);

    QWidget::paintEvent(event);
}

void ColorWheel::resizeEvent(QResizeEvent* event){
    Q_UNUSED(event);
    this->setGeometry(0, 0, m_Size.width(), m_Size.height());
}

void ColorWheel::mouseMoveEvent(QMouseEvent* event){
    m_LastCursorPos = event->pos();

    *this->m_SelectedColor = GetColorAtPoint(m_LastCursorPos, m_Radius);
    OnColorChange();
}

void ColorWheel::mousePressEvent( QMouseEvent* event){
    m_LastCursorPos = event->pos();

    *this->m_SelectedColor = GetColorAtPoint(m_LastCursorPos, m_Radius);
    OnColorChange();
}

void ColorWheel::OnColorChange()
{
    m_ShadeCircle.SetRelativeColor(*m_SelectedColor.get());
    this->repaint();
    emit ColorChanged();
}

OklabColorWheel::OklabColorWheel(QWidget *parent)
    : ColorWheel(parent)
{
    m_ParentRect = parent->rect();
}

double OklabColorWheel::GetChroma() const
{
    return m_CurrentChroma;
}

void OklabColorWheel::SetChroma(double chroma)
{
    this->m_CurrentChroma = chroma;
}

double OklabColorWheel::GetLightness() const
{
    return m_CurrentLightness;
}

void OklabColorWheel::SetLightness(double lightness)
{
    this->m_CurrentLightness = lightness;
}

void OklabColorWheel::OnPaintEvent(QPainter& painter)
{
    const auto parent_center = m_ParentRect.center();

    auto gradient = QConicalGradient(parent_center, 0);

    const int num_steps = 360;

    for (int i = 0; i < num_steps; ++i) {
        double normalized_angle = (double) i / (double) (num_steps - 1);

        const double a = m_CurrentChroma * std::cos(2 * M_PI * normalized_angle);
        const double b = m_CurrentChroma * std::sin(2 * M_PI * normalized_angle);
        gradient.setColorAt(normalized_angle, Oklab(m_CurrentLightness,a,b));
    }

    QPen pen(QBrush(gradient), 20);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);

    painter.setBrush(gradient);
    painter.drawEllipse(m_ParentRect);
}

QColor OklabColorWheel::GetColorAtPoint(QPoint point, float radius) const
{
    double angle = (qAtan2(point.x() - radius, point.y() - radius) + M_PI);
    angle = angle + M_PI_2;

    const double normalized_angle = std::fmod(angle, 2 * M_PI) / (2 * M_PI);

    const double a_component = m_CurrentChroma * std::cos(normalized_angle * 2 * M_PI);
    const double b_component = m_CurrentChroma * std::sin(normalized_angle * 2 * M_PI);

    return Oklab(m_CurrentLightness, a_component, b_component);
}

HsvColorWheel::HsvColorWheel(QWidget *parent) : ColorWheel(parent)
{
    m_ParentRect = parent->rect();
}

double HsvColorWheel::GetSaturation() const
{
    return m_Saturation;
}

void HsvColorWheel::SetSaturation(double saturation)
{
    this->m_Saturation = saturation;
}

double HsvColorWheel::GetBrightness() const
{
    return m_Brightness;
}

void HsvColorWheel::SetBrightness(double brightness)
{
    this->m_Brightness = brightness;
}

void HsvColorWheel::OnPaintEvent(QPainter& painter)
{
    const auto parent_center = m_ParentRect.center();

    auto gradient = QConicalGradient(parent_center, 90);

    const int num_steps = 360;

    for (int i = 0; i < num_steps; ++i) {
        const double normalized_angle = (double) i / (double) (num_steps - 1);

        auto color = QColor::fromHsvF(normalized_angle, m_Saturation, m_Brightness);
        gradient.setColorAt(normalized_angle, color);
    }

    QPen pen(QBrush(gradient), 20);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);

    painter.setBrush(gradient);
    painter.drawEllipse(m_ParentRect);
}

QColor HsvColorWheel::GetColorAtPoint(QPoint point, float radius) const
{
    const auto hue = (qAtan2(point.x() - radius, point.y() - radius) + M_PI) / (2.f * M_PI);
    auto saturation = qSqrt(qPow(point.x() - radius, 2) + qPow(point.y() - radius, 2)) / radius;
    const auto brightness = m_Brightness;

    saturation = qBound(0.0, saturation, 1.0);

    return QColor::fromHsvF(hue, saturation, brightness);
}
