#include "AddIcon.hpp"
#include <QMouseEvent>

#include "Utils.hpp"

AddIcon::AddIcon(QWidget *parent, QSize size) : QLabel(parent), m_Size(size) , m_PalleteNamePopup(parent){
    QPixmap add_icon_pixmap(":/resources/add_icon.png");
    this->setPixmap(add_icon_pixmap);
    this->setGeometry(0, 0, m_Size.width(), m_Size.height());
    this->setToolTip("Add color");
    this->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    connect(&m_PalleteNamePopup, &QLineEdit::returnPressed, this, [=](){
        emit IsNamed(m_PalleteNamePopup.text());
        m_PalleteNamePopup.clear();
        m_PalleteNamePopup.close();
    });

    connect(&m_PalleteNamePopup, &PalleteNameSpecifierPopup::PressedOutside, this, [=](){
        emit WasNotNamed();
        m_PalleteNamePopup.clear();
        m_PalleteNamePopup.close();
    });
}

void AddIcon::OnParentGeometryUpdate(QRect rect){
    m_PalleteNamePopup.setGeometry(rect.center().x() - (m_PalleteNamePopup.width() / 2 ), rect.center().y() - (m_PalleteNamePopup.height() / 2 ), m_PalleteNamePopup.width(), m_PalleteNamePopup.height() );
}

void AddIcon::SetPlaceHolderText(QString placeholder)
{
    m_PalleteNamePopup.setPlaceholderText(placeholder);
}

void AddIcon::mousePressEvent(QMouseEvent *event){
    auto pos = event->pos();

    if(event->button() == Qt::MouseButton::LeftButton){
        QRect rect(pos.x(), pos.y(), 1, 1);
        if(rect.intersects(this->rect())){
            m_PalleteNamePopup.setVisible(true);
            emit IsLeftClicked();
        }
    }
}

void AddIcon::closeEvent(QCloseEvent *event){
    QLabel::closeEvent(event);
}

PalleteNameSpecifierPopup::PalleteNameSpecifierPopup(QWidget *parent) : QLineEdit(parent){
    this->setWindowFlags(Qt::Popup |  Qt::FramelessWindowHint);
    this->setWindowModality(Qt::WindowModal);

    QFont font = this->font();
    font.setPointSize(Utils::Font::GetAdaptiveFontSize(24));
    this->setFont(font);

    auto size = this->parentWidget()->size();

    qint16 new_width{static_cast<qint16>(size.width() * 0.5f)},new_height{static_cast<qint16>(size.height() * 0.15f)};
    this->setGeometry(this->parentWidget()->geometry().center().x() - (new_width / 2 ),this->parentWidget()->geometry().center().y() - (new_height / 2 ), new_width, new_height);
    this->setVisible(false);

    connect(this,&QLineEdit::textChanged,this,[=](){

        this->update();
        updateGeometry();
    });
}

void PalleteNameSpecifierPopup::mousePressEvent(QMouseEvent *event){
    auto pos = event->pos();

    if(event->button() == Qt::MouseButton::LeftButton){
        QRect rect(pos.x(), pos.y(), 30, 30);
        if(!rect.intersects(this->rect())){
            emit PressedOutside();
            return;
        }
    }
}
