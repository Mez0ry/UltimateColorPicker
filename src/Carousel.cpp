#include "Carousel.hpp"
#include "ColorWheel.hpp"
#include "EllipseButton.hpp"
#include "Utils.hpp"
#include "mainwindow.h"

#include <QGraphicsOpacityEffect>
#include <QTimer>

Carousel::Carousel(QWidget* parent)
    : QWidget(parent),
    m_CurrentIndex(-1),
    m_PrevIndex(0)
{
    auto main_window = static_cast<MainWindow*>(Utils::GetMainWindow());
    auto ui = main_window->GetUi();

    m_Wheels.push_back(std::make_shared<HsvColorWheel>(parent));
    m_Wheels.push_back(std::make_shared<OklabColorWheel>(parent));

    for(auto& wid : m_Wheels){
        wid->installEventFilter(this);
    }

    EllipseButton* hsv_wheel_btn = new EllipseButton();
    EllipseButton* oklab_wheel_btn = new EllipseButton();

    m_Buttons.push_back(hsv_wheel_btn);
    m_Buttons.push_back(oklab_wheel_btn);

    auto background_layout_widget = ui->frame_3;

    QHBoxLayout* actual_buttons_layout = new QHBoxLayout(this);
    actual_buttons_layout->setSpacing(2);
    actual_buttons_layout->addStretch();
    actual_buttons_layout->addWidget(hsv_wheel_btn, 0, Qt::AlignCenter);
    actual_buttons_layout->addWidget(oklab_wheel_btn, 0, Qt::AlignCenter);
    actual_buttons_layout->addStretch();

    background_layout_widget->setLayout(actual_buttons_layout);

    int index = 0;
    for(auto& color_wheel : m_Buttons){
        connect(color_wheel, &QPushButton::clicked, this, std::bind(&Carousel::SelectItemAt, this, index));
        ++index;
    }

    m_CarouselTimer = new QTimer(this);

    QObject::connect(m_CarouselTimer, &QTimer::timeout, [&](){
        NextItem();
    });

    SelectItemAt(0);
}

Carousel::~Carousel() {
    for(auto& wid : m_Wheels){
        wid->removeEventFilter(this);
    }
}

bool Carousel::eventFilter(QObject* obj, QEvent* ev)
{
    foreach (auto color_wheel_widget, m_Wheels) {
        if(!color_wheel_widget)
            continue;

        if(obj == color_wheel_widget.get() && ev->type() == QEvent::MouseButtonPress && color_wheel_widget->inherits("ColorWheel")) {
            obj->event(ev);
            m_CarouselTimer->start(8000);
            return true;
        }
    };

    return QWidget::eventFilter(obj, ev);
}

void Carousel::SelectItemAt(int index) {
    if(m_CurrentIndex == index)
        return;

    m_PrevIndex = m_CurrentIndex;
    m_CurrentIndex = qBound(0, index, m_Wheels.size() - 1);

    for(auto button_index = 0; button_index < m_Buttons.size(); button_index++){
        m_Buttons[button_index]->StartAnimation((button_index != index) ? 0.0 : 1.0, 150);
    }

    ChangeCurrentItem();
}

void Carousel::PreviousItem() {
    if (m_CurrentIndex > 0){
        m_CurrentIndex--;
    } else {
        m_CurrentIndex = m_Wheels.size() - 1;
    }

    ChangeCurrentItem();
}

void Carousel::NextItem() {
    if (m_CurrentIndex < static_cast<int>(m_Wheels.size() - 1)) {
        m_CurrentIndex++;
    } else {
        m_CurrentIndex = 0;
    }

    ChangeCurrentItem();
}

void Carousel::ChangeCurrentItem() {

    auto prev_wheel = m_Wheels.value(m_PrevIndex).get();
    auto curr_wheel = m_Wheels.value(m_CurrentIndex).get();

    QPropertyAnimation* fade_in_anim = new QPropertyAnimation(curr_wheel, "ScaleFactor");
    fade_in_anim->setDuration(250);
    fade_in_anim->setStartValue(0.0);
    fade_in_anim->setEndValue(1.0);

    fade_in_anim->setEasingCurve(QEasingCurve::OutCubic);
    fade_in_anim->start(QAbstractAnimation::DeletionPolicy::DeleteWhenStopped);

    if(prev_wheel){
        prev_wheel->setVisible(false);
    }

    curr_wheel->setVisible(true);
    curr_wheel->raise();
    curr_wheel->GetShadeCircle().raise();

    for(auto button_index = 0; button_index < m_Buttons.size(); button_index++){
        m_Buttons[button_index]->StartAnimation((button_index != m_CurrentIndex) ? 0.0 : 1.0, 150);
    }

    m_CarouselTimer->start(8000);
}
