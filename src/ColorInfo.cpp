#include "ColorInfo.hpp"
#include "mainwindow.h"

#include <memory>

ColorInfo::ColorInfo(std::shared_ptr<QColor> color, QWidget *parent) : QWidget(parent), m_RgbaFormat("rgba(%r, %g, %b, %a)")
{
    SetColorContext(color);

    auto main_window = static_cast<MainWindow*>(Utils::GetMainWindow());
    auto ui = main_window->GetUi();

    if(!ui){
        qCritical() << "ui context is nullptr" << '\n';
    }

    int half_height = ui->rgba_LineEdit->height() / 2.2;
    qBound(5, half_height, 12);

    QString border_radius = QString("border-radius: %1px;").arg(half_height);

    ui->rgba_LineEdit->setStyleSheet(border_radius);
    ui->hex_value->setStyleSheet(border_radius);

    QAction * rgba_line_edit_action = ui->rgba_LineEdit->addAction(QIcon(":/resources/copy_icon_1.png"), QLineEdit::TrailingPosition);

    connect(rgba_line_edit_action,&QAction::triggered,this,[=](bool checked){
        Q_UNUSED(checked);

        QClipboard *clipboard = QApplication::clipboard();

        if(!clipboard){
            return;
        }

        if(ui->rgba_LineEdit && !ui->rgba_LineEdit->text().isEmpty()){
            QString r,g,b,a;

            r = QString::number(this->m_ColorContext->red());
            g = QString::number(this->m_ColorContext->green());
            b = QString::number(this->m_ColorContext->blue());
            a = QString::number(this->m_ColorContext->alpha());

            QString formatted_rgb = m_RgbaFormat.replace("%r","%1").replace("%g","%2").replace("%b","%3").replace("%a","%4").arg(r,g,b,a);
            clipboard->setText(formatted_rgb,QClipboard::Mode::Clipboard);
        }
    });

    connect(ui->rgba_LineEdit, &QLineEdit::editingFinished,this,[=](){
        QString new_text = ui->rgba_LineEdit->text();

        QMessageBox msgBox;

        if(!new_text.contains("%r") || !new_text.contains("%g") || !new_text.contains("%b") || !new_text.contains("%a")){
            msgBox.setText("Incorrect rgb format, you didnt applied one or all color components to your format");
            msgBox.setInformativeText("Do you want to set default format?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            if(msgBox.exec() == QMessageBox::Yes){
                m_RgbaFormat = "rgba(%r, %g, %b, %a)";
                ui->rgba_LineEdit->setText(m_RgbaFormat);
                return;
            }
        }

        m_RgbaFormat = new_text;
    });


    QAction * hex_value_action = ui->hex_value->addAction(QIcon(":/resources/copy_icon_1.png"), QLineEdit::TrailingPosition);

    connect(hex_value_action,&QAction::triggered,this,[=](bool checked){
        Q_UNUSED(checked);

        QClipboard *clipboard = QApplication::clipboard();

        if(!clipboard){
            return;
        }

        clipboard->setText(ui->hex_value->text(),QClipboard::Mode::Clipboard);
    });
}

ColorInfo::~ColorInfo()
{
    DisconnectAll();
}

void ColorInfo::SetColorContext(std::shared_ptr<QColor> color)
{
    if(color == m_ColorContext){
        return;
    }

    if(!m_Connections.empty()){
        DisconnectAll();
    }

    m_ColorContext = color;
    SetupConnections();
}

 std::shared_ptr<QColor> ColorInfo::GetColorContext() const
{
    return m_ColorContext;
}

void ColorInfo::SetupColorInfo()
{
    auto main_window = static_cast<MainWindow*>(Utils::GetMainWindow());
    auto ui = main_window->GetUi();

    if(!ui){
        qCritical() << "ui context is nullptr" << '\n';
    }

    ui->hex_value->setText(m_ColorContext->name());

    ui->r_hSlider->setValue(m_ColorContext->red());
    ui->g_hSlider->setValue(m_ColorContext->green());
    ui->b_hSlider->setValue(m_ColorContext->blue());
    ui->a_hSlider->setValue(m_ColorContext->alpha());

    ui->r_LineEdit->setText(QString::number(m_ColorContext->red()));
    ui->g_LineEdit->setText(QString::number(m_ColorContext->green()));
    ui->b_LineEdit->setText(QString::number(m_ColorContext->blue()));
    ui->a_LineEdit->setText(QString::number(m_ColorContext->alpha()));

    ui->rgba_LineEdit->setText(m_RgbaFormat);
}

void ColorInfo::SetupConnections()
{
    /**@brief
     * @TODO set color outside of this class for shades/hue/color wheel
    */

    auto main_window = static_cast<MainWindow*>(Utils::GetMainWindow());
    auto ui = main_window->GetUi();

    if(!ui){
        qCritical() << "ui context is nullptr" << '\n';
    }

    m_Connections.push_back(connect(ui->r_hSlider, &QSlider::valueChanged, this,[=](int red){
        this->m_ColorContext->setRed(red);
        ui->r_hSlider->setValue(this->m_ColorContext->red());
        SetupColorInfo();
        emit InfoChanged();
    }));

    m_Connections.push_back(connect(ui->g_hSlider, &QSlider::valueChanged, this,[=](int green){
        this->m_ColorContext->setGreen(green);
        ui->g_hSlider->setValue(this->m_ColorContext->green());
        emit InfoChanged();
    }));

    m_Connections.push_back(connect(ui->b_hSlider, &QSlider::valueChanged, this,[=](int blue){
        this->m_ColorContext->setBlue(blue);
        ui->b_hSlider->setValue(this->m_ColorContext->blue());

        emit InfoChanged();
    }));

    m_Connections.push_back(connect(ui->a_hSlider, &QSlider::valueChanged, this,[=](int alpha){
        this->m_ColorContext->setAlpha(alpha);
        ui->b_hSlider->setValue(this->m_ColorContext->alpha());

        emit InfoChanged();
    }));

    m_Connections.push_back(connect(ui->r_LineEdit,&QLineEdit::textChanged,this,[=](QString new_string){
        this->m_ColorContext->setRed(qBound(0,new_string.toInt(),255));
        ui->r_LineEdit->setText(new_string);
        emit InfoChanged();
    }));

    m_Connections.push_back(connect(ui->g_LineEdit,&QLineEdit::textChanged,this,[=](QString new_string){
        this->m_ColorContext->setGreen(qBound(0,new_string.toInt(),255));
        ui->g_LineEdit->setText(new_string);
        emit InfoChanged();
    }));

    m_Connections.push_back(connect(ui->b_LineEdit,&QLineEdit::textChanged,this,[=](QString new_string){
        this->m_ColorContext->setBlue(qBound(0,new_string.toInt(),255));
        ui->b_LineEdit->setText(new_string);
        emit InfoChanged();
    }));

    m_Connections.push_back(connect(ui->a_LineEdit,&QLineEdit::textChanged,this,[=](QString new_string){
        this->m_ColorContext->setAlpha(qBound(0,new_string.toInt(),255));
        ui->a_LineEdit->setText(new_string);

        emit InfoChanged();
    }));

    m_Connections.push_back(connect(
        ui->hex_value,&QLineEdit::editingFinished,[=](){
            auto new_hex_text = ui->hex_value->text();
            QRegularExpression hex_is_valid("^#(?:[0-9a-fA-F]{3}){1,2}$");
            if(!hex_is_valid.match(new_hex_text).hasMatch()){
                qWarning() << "invalid hex code" << '\n';
                return;
            }

            *m_ColorContext = QColor::fromString(new_hex_text);
            emit InfoChanged();
    }));
}

void ColorInfo::DisconnectAll()
{
    for(auto& connection : m_Connections){
        QObject::disconnect(connection);
    }
    m_Connections.clear();
}
