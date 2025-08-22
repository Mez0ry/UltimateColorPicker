#include "ColorWheel.hpp"

#include "mainwindow.h"
ColorWheel::ColorWheel(QWidget *parent, QColor selected_color, quint16 margin) : QWidget(parent), m_ShadeCircle(this), m_ColorInfo(m_ShadeCircle.GetSelectedShade(),this){
    this->setMouseTracking(false);

    auto main_window = static_cast<MainWindow*>(Utils::GetMainWindow());
    auto ui = main_window->GetUi();

    if(!ui){
        qCritical() << "ui context is nullptr" << '\n';
    }

    m_Radius = 0;
    m_SelectedColor = std::make_shared<QColor>(selected_color);
    this->m_ShadeCircle.SetShadeColor(std::make_shared<QColor>(*m_SelectedColor));

    this->m_Hue = m_SelectedColor->hueF();
    this->m_Saturation = m_SelectedColor->saturationF();
    this->m_Brightness = m_SelectedColor->valueF();
    this->m_Margin = margin;

    auto size_policy = QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    size_policy.setHeightForWidth(true);
    this->setSizePolicy(size_policy);

    this->setGeometry(0, 0, 150,150);
    connect(&m_ColorInfo,&ColorInfo::InfoChanged,this,[=](){
        this->m_ColorInfo.SetupColorInfo();
        this->update();
    });

    connect(&m_ShadeCircle,&ShadeCircle::ShadeColorChanged,this,[=](){
        this->update();
    });

    connect(&m_ShadeCircle,&ShadeCircle::OnShadeChanged,this,[=]( std::shared_ptr<QColor> new_color){
        m_ColorInfo.SetColorContext(new_color);
        m_ColorInfo.SetupColorInfo();

        m_ColorCombo.SetBaseColor(new_color.get());
        m_ShadeCircle.update();
    });

    connect(this, &ColorWheel::SigColorChanged, this,[=](){
        m_ColorInfo.SetColorContext(m_SelectedColor);
        m_ColorInfo.SetupColorInfo();
        m_ColorCombo.SetBaseColor(m_SelectedColor.get());
        this->update();
    });

    OnColorChange();
}

ColorWheel::~ColorWheel(){

}

void ColorWheel::paintEvent(QPaintEvent * event){
    Q_UNUSED(event);

    auto center = QPointF(this->width() / 2, this->height() / 2);
    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::VerticalSubpixelPositioning | QPainter::LosslessImageRendering);
    painter.setViewport(m_Margin, m_Margin, this->width() - 2 * m_Margin , this->height() - 2 * m_Margin);

    auto hsv_grad = QConicalGradient(center, 90);

    for(float deg = 0; deg < 360; deg += 1.f){
        auto color = QColor::fromHsvF(deg / 360, 1, m_Brightness.GetBrightness());
        hsv_grad.setColorAt(deg / 360, color);
    }

    int arcLengthApproximation = 20 + 20 / 3;

    QPen pen( QBrush(hsv_grad), 20);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);

    QRect drawingRect;
    drawingRect.setX(rect().x() + 20);
    drawingRect.setY(rect().y() + 20);
    drawingRect.setWidth(rect().width() - 20 * 2);
    drawingRect.setHeight(rect().height() - 20 * 2);

    painter.drawArc(drawingRect, 390 * 16 - arcLengthApproximation , 16);

    auto val_grad = QRadialGradient(center, 80);
    val_grad.setColorAt(0.0f, *m_ShadeCircle.GetSelectedShade());
    val_grad.setColorAt(1.f, *m_ShadeCircle.GetSelectedShade());

    painter.setBrush(val_grad);
    painter.drawEllipse(this->rect());

    painter.setPen(Qt::black);
    painter.setBrush(*m_SelectedColor.get());

    auto line = QLineF::fromPolar((m_Radius * m_Saturation.GetSaturation() + 75), 360 * m_Hue.GetHue() + 90);

    line.translate(drawingRect.center());
    painter.drawEllipse(line.p2(), 7, 7);

    this->m_ShadeCircle.SetRelativeColor(*m_SelectedColor);
}

void ColorWheel::resizeEvent(QResizeEvent* event){
    Q_UNUSED(event);
    this->setGeometry(0, 0, 150,150);
}

void ColorWheel::mouseMoveEvent(QMouseEvent* event){
    auto pos = event->pos();

    auto hsv = Utils::GetHsvFrom(pos,m_Radius,this->m_Brightness);

    m_Hue = hsv.hue;
    m_Saturation = hsv.saturation;
    m_Brightness = hsv.brightness;

    OnColorChange();
}

void ColorWheel::mousePressEvent(QMouseEvent* event){

    auto pos = event->pos();

    auto hsv = Utils::GetHsvFrom(pos,m_Radius,this->m_Brightness);

    m_Hue = hsv.hue;
    m_Saturation = hsv.saturation;
    m_Brightness = hsv.brightness;

    OnColorChange();
}

void ColorWheel::OnColorChange()
{
    this->m_SelectedColor->setHsvF(m_Hue.GetHue(),m_Saturation.GetSaturation(),m_Brightness.GetBrightness(),m_SelectedColor->alphaF());
    m_ShadeCircle.SetRelativeColor(*m_SelectedColor.get());
    this->repaint();
    emit SigColorChanged();
}
