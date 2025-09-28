#include "DeletePopup.hpp"
#include <QMouseEvent>
#include "Utils.hpp"

DeletePopup::DeletePopup(QWidget *parent) : QLabel(parent){
    this->setVisible(false);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    this->setWindowModality(Qt::NonModal);

    QFont font = this->font();
    font.setPointSize(Utils::Font::GetAdaptiveFontSize(8));
    this->setFont(font);
    this->setText("Delete");

    this->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    this->setMinimumWidth(10);
    this->setMinimumHeight(10);
    this->setMaximumHeight(20);
    this->setMaximumWidth(70);
}

void DeletePopup::OnRightClick(int border_length){
    auto parent_geom_rect = this->parentWidget()->geometry();
    auto parent_geom_pos = this->parentWidget()->mapToGlobal(QPoint(0,0));

    constexpr auto popup_angle = qDegreesToRadians(qreal(320));

    const auto r = (parent_geom_rect.width() / 2) + border_length;
    const int x = int(r * (qFastCos(popup_angle)));
    const int y = int(-r * (qFastSin(popup_angle)));

    auto pos = parent_geom_pos + QPoint(x,y);
    this->move(pos);

    this->setVisible(true);
    this->show();
}

void DeletePopup::mousePressEvent(QMouseEvent *event){
    auto pos = event->pos();

    if(event->button() == Qt::MouseButton::LeftButton){
        QRect rect(pos.x(),pos.y(),this->width(),this->height());

        if(!rect.intersects(this->rect())){
            this->close();
            return;
        }

        this->close();
        emit LeftButtonClicked();
    }
}

void DeletePopup::keyReleaseEvent(QKeyEvent *){
    this->close();
}
