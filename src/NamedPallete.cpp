#include "NamedPallete.hpp"
#include <QSettings>
#include <QPainter>
#include <QMouseEvent>

#include "Utils.hpp"


NamedPallete::NamedPallete(const QString &pallete_name, QJsonObject color_values, QWidget *parent) : QLabel(parent),m_Size(32,32), m_PalleteName(pallete_name), m_ColorValues(color_values), m_DeletePopup(this){
    this->setToolTip(m_PalleteName);
    this->setGeometry(0,0,m_Size.width(),m_Size.height());

    this->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));

    this->setMaximumSize(QSize(m_Size.width() + 122,m_Size.height() + 122));
    this->setMinimumSize(m_Size);

    connect(&m_DeletePopup,&DeletePopup::LeftButtonClicked,this,[=](){
        QSettings settings("Mezory", "ColorPicker");
        settings.beginGroup("ConfigFiles");
        auto config_path = settings.value("color_pallete_config").toString();
        settings.endGroup();

        auto json_doc = Utils::Json::LoadJson(config_path);
        auto root_obj = json_doc.object();
        root_obj.remove(m_PalleteName);
        json_doc.setObject(root_obj);

        Utils::Json::SaveJson(json_doc,config_path);

        emit JsonConfigUpdate(this);
    });
}

void NamedPallete::Serialize(QJsonDocument doc, const QString &file_path){
    NamedPallete::Serialize(this,doc,file_path);
}

void NamedPallete::Serialize(const NamedPallete *pallete, QJsonDocument &doc, const QString &file_path){
    const auto& color_values = pallete->GetColors();
    const auto& pallete_name = pallete->GetPalleteName();

    QJsonObject  root_obj = doc.object();

    if(root_obj.contains(pallete_name)){
        auto it = root_obj.find(pallete_name);
        *it = color_values;
    }else{
        root_obj.insert(pallete_name, color_values);
    }

    doc.setObject(root_obj);
    Utils::Json::SaveJson(doc,file_path);
}

 QJsonObject NamedPallete::GetColors() const{
    return m_ColorValues;
}

 void NamedPallete::SetColors(QJsonObject colors)
 {
    m_ColorValues = colors;
 }

const QString &NamedPallete::GetPalleteName() const {return m_PalleteName;}

void NamedPallete::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::MouseButton::LeftButton){
        emit Clicked();
    }else if(event->button() == Qt::MouseButton::RightButton){
        m_DeletePopup.OnRightClick();
    }
    QLabel::mousePressEvent(event);
}

void NamedPallete::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRadialGradient gradient(this->rect().center(),16);
    double fill_dx =  ((double)m_ColorValues.size() / (double)m_Size.height());
    int iter = 0;
    for(auto color_value : m_ColorValues){
        QColor color(color_value.toString());
        double pos_at = (double)(iter * fill_dx);
        if(pos_at <= 1.0)
            gradient.setColorAt(pos_at,color);
        ++iter;
    }
    painter.setBrush(gradient);

    painter.drawEllipse(this->rect().center(), 16, 16 );
}
