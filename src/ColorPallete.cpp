#include "ColorPallete.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>
#include <QPainterPath>
#include <QLayout>
#include <QVBoxLayout>
#include <QGraphicsBlurEffect>
#include <QGridLayout>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QResizeEvent>

#include "NamedPallete.hpp"
#include "AddIcon.hpp"
#include "Utils.hpp"

#include "GridLayout.hpp"
#include "ColoredEllipse.hpp"

ColorPallete::ColorPallete(QWidget *parent) : QWidget(parent)
{
    this->setStyleSheet("background-color: #1B1D23;");

    this->setWindowFlag(Qt::Window);
    const QRect desktop_rect = screen()->availableGeometry();

    constexpr float width_percentage = 0.4f;
    constexpr float height_percentage = 0.26f;

    m_Size.setWidth( desktop_rect.width() * width_percentage);
    m_Size.setHeight(desktop_rect.height() * height_percentage);

    QPoint center = desktop_rect.center();
    this->setGeometry(center.x() - width() * 0.5, center.y() - height() * 0.5,m_Size.width(),m_Size.height());

    this->setMinimumSize(500,500);

    const QRect rect{0,0,62,m_Size.height()};
    constexpr quint8 spacing = 22;

    m_BackgroundVBoxLayout = new QFrame(this);
    m_BackgroundVBoxLayout->setMinimumWidth(62);
    m_BackgroundVBoxLayout->setMaximumWidth(100);
    m_BackgroundVBoxLayout->setMaximumHeight(desktop_rect.height());

    m_BackgroundVBoxLayout->setWindowFlags(Qt::NoTitleBarBackgroundHint);

    m_BackgroundVBoxLayout->setGeometry(rect);
    m_BackgroundVBoxLayout->setStyleSheet(QString("background-color: #15161B;"));

    m_Content = new QFrame(this);
    m_Content->setMinimumWidth(this->minimumWidth());
    m_Content->setMaximumWidth(this->maximumWidth());

    m_Content->setStyleSheet(QString("background-color: #1B1D23;"));
    m_Content->setGeometry(rect.x() + rect.width(),rect.y(),this->width() - rect.width(), this->height());

    const QPoint grid_size(10,100);

    auto* const grid_layout = new GridLayout(grid_size.y(),grid_size.x(),m_Content);

    grid_layout->setContentsMargins(11,15,0,0);
    grid_layout->setGeometry(QRect(0,0,this->width() - rect.width(), this->height()));
    grid_layout->setOriginCorner(Qt::TopLeftCorner);
    grid_layout->setColumnMinimumWidth(0,44);
    m_Content->setLayout(grid_layout);

    m_ScrollArea = new QScrollArea(this);
    m_ScrollArea->setStyleSheet("QScrollBar:vertical{ border: none; background: rgb(34,36,44); }");
    m_ScrollArea->setWidgetResizable(true);
    m_ScrollArea->setBackgroundRole(QPalette::Dark);
    m_ScrollArea->setGeometry(rect);

    m_BackgroundVBoxLayout->setLayout(new QVBoxLayout());

    m_ScrollArea->setWidget(m_BackgroundVBoxLayout);
    m_BackgroundVBoxLayout->layout()->setGeometry(rect);

    m_BackgroundVBoxLayout->layout()->setSpacing(spacing);
    m_BackgroundVBoxLayout->layout()->setContentsMargins(0,spacing / 2,0,0);

    m_AddIcon = new AddIcon(this);
    m_AddIcon->SetPlaceHolderText("Sun Bliss");

    QSettings settings("Mezory", "ColorPicker");

    settings.beginGroup("ConfigFiles");
    const auto color_pallete_cfg_path = settings.value("color_pallete_config").toString();
    auto json_doc = Utils::Json::LoadJson(color_pallete_cfg_path);
    settings.endGroup();

    auto root_obj = json_doc.object();

    grid_layout->setSizeConstraint(QLayout::SetMinimumSize);

    auto on_click_slot = [=](auto* pallete_ptr){
        grid_layout->Clear();

        auto colors = pallete_ptr->GetColors();
        auto next_ins_idx = grid_layout->GetNextInsertionPos();

        for(auto color : colors){
            next_ins_idx = grid_layout->GetNextInsertionPos();

            grid_layout->PushWidget(new ColoredEllipse(QColor( color.toString())),1,1, Qt::AlignCenter);
            grid_layout->setSpacing(0);
            grid_layout->setVerticalSpacing(0);
        }

        auto* color_add_icon = new AddIcon(this);

        color_add_icon->SetPlaceHolderText("#FFFFFF");

        connect(color_add_icon,&AddIcon::IsLeftClicked,this,[=](){
            //window_blur->setEnabled(true);
        });

        connect(color_add_icon,&AddIcon::WasNotNamed,this,[=](){
            //window_blur->setEnabled(false);
            this->update();
        });

        connect(color_add_icon,&AddIcon::IsNamed,this,[=](QString new_text){
            auto prev_index = grid_layout->count() - 1;
            auto icon_widget = grid_layout->itemAt(prev_index)->widget();
            if(icon_widget){
                grid_layout->removeWidget(icon_widget);
            }

            auto* new_widget = new ColoredEllipse(QColor(new_text));
            grid_layout->PushWidget(new_widget,1,1, Qt::AlignCenter);
            grid_layout->PushWidget(icon_widget,1,1, Qt::AlignCenter);

            auto new_colors = pallete_ptr->GetColors();
            QString new_index_str =  QString::number(colors.size() + 1);

            if(new_colors.size() > 0){
                auto last_name = (pallete_ptr->GetColors().end() - 1).key();
                auto index_of = last_name.indexOf("_");

                auto left_part_str = last_name.left(index_of + 1);
                new_colors.insert(left_part_str + new_index_str,new_text);
            }else{
                new_colors.insert("color_" + new_index_str,new_text);
            }

            pallete_ptr->SetColors(new_colors);

            QSettings settings("Mezory", "ColorPicker");
            settings.beginGroup("ConfigFiles");
            auto pallete_path = settings.value("color_pallete_config").toString();
            settings.endGroup();
            pallete_ptr->Serialize(Utils::Json::LoadJson(pallete_path),pallete_path);
        });

        grid_layout->PushWidget(color_add_icon,1,1, Qt::AlignCenter);

        grid_layout->setRowStretch(next_ins_idx.y() + 2,1);
        grid_layout->setColumnStretch(grid_size.x(),1);

        grid_layout->setSpacing(16);
        qInfo() << "clicked: " << pallete_ptr<<'\n';
    };

    auto idx = 0;

    for(auto json : root_obj){
        const auto key = root_obj.keys()[idx];
        const auto color_values = json.toObject();

        auto* const pallete_ptr = new NamedPallete(key,color_values,this);
        m_BackgroundVBoxLayout->layout()->addWidget(pallete_ptr);

        connect(pallete_ptr,&NamedPallete::JsonConfigUpdate,[=](NamedPallete* const pallete_to_remove){
           m_BackgroundVBoxLayout->layout()->removeWidget(pallete_to_remove);
           delete pallete_to_remove;

           this->update();
        });

        auto on_click_cb = std::bind(on_click_slot,pallete_ptr);
        connect(pallete_ptr,&NamedPallete::Clicked,on_click_cb);

        ++idx;
    }

    auto* const qvbox_layout = qobject_cast<QVBoxLayout*>(m_BackgroundVBoxLayout->layout());
    qvbox_layout->addWidget(m_AddIcon,0,Qt::AlignHCenter);

    qvbox_layout->addStretch();

    QGraphicsBlurEffect* window_blur = new QGraphicsBlurEffect();

    window_blur->setBlurRadius(3);
    m_BackgroundVBoxLayout->setGraphicsEffect(window_blur);
    window_blur->setEnabled(false);

    connect(m_AddIcon,&AddIcon::IsLeftClicked,this,[=](){
        window_blur->setEnabled(true);

    });

    connect(m_AddIcon,&AddIcon::WasNotNamed,this,[=](){
        window_blur->setEnabled(false);
        this->update();
    });

    connect(m_AddIcon,&AddIcon::IsNamed,this,[=](QString new_text){
        const int insert_index = m_BackgroundVBoxLayout->layout()->count() - 2; // insert index thats before 'add icon' and previously added stretch

        auto* const new_widget = new NamedPallete(new_text,QJsonObject(),this);
        qvbox_layout->insertWidget(insert_index,new_widget);

        window_blur->setEnabled(false);
        QSettings settings("Mezory", "ColorPicker");
        settings.beginGroup("ConfigFiles");
        auto pallete_path = settings.value("color_pallete_config").toString();
        settings.endGroup();
        new_widget->Serialize(Utils::Json::LoadJson(pallete_path),pallete_path);

        connect(new_widget,&NamedPallete::JsonConfigUpdate,[=](NamedPallete* pallete_to_remove){
            m_BackgroundVBoxLayout->layout()->removeWidget(pallete_to_remove);
            m_BackgroundVBoxLayout->update();
            pallete_to_remove->setVisible(false);
            this->update();

            pallete_to_remove->deleteLater();
            return;
        });

        auto on_click_cb = std::bind(on_click_slot,new_widget);
        connect(new_widget,&NamedPallete::Clicked, on_click_cb);
    });
}

void ColorPallete::OnButtonClicked()
{
    this->show();
}

void ColorPallete::moveEvent(QMoveEvent *event)
{
    this->m_AddIcon->OnParentGeometryUpdate(this->geometry());
    QWidget::moveEvent(event);
}

void ColorPallete::resizeEvent(QResizeEvent *event)
{
    OnResize(event->size());
    QWidget::resizeEvent(event);
}

void ColorPallete::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
}

void ColorPallete::OnResize(QSize new_size)
{
    m_Size = new_size;

    double wx =  m_Size.width() * 0.104;
    wx = std::clamp(wx,(double)62,(double)100);

    const QRect rect(0,0,wx,m_Size.height());

    m_BackgroundVBoxLayout->setGeometry(rect);
    m_BackgroundVBoxLayout->layout()->setGeometry(rect);
    m_BackgroundVBoxLayout->update();

    m_ScrollArea->setGeometry(rect);

    m_Content->setGeometry(rect.x() + rect.width(),rect.y(),this->width(), this->height() / 2);

    auto* const grid_layout = qobject_cast<GridLayout*>(m_Content->layout());
    auto grid_size = grid_layout->GetGridSize();
    quint32 content_width = m_Content->width();

    if(!grid_layout->IsEmpty()){
        const auto total_width = grid_layout->GetRowTotalWidth();

        const auto widget_width = grid_layout->GetMaxWidth() / grid_layout->count();
        const auto max_width = grid_layout->GetMaxWidth() - (widget_width * 4);

        if(total_width + 150 < content_width && total_width  < max_width){
            const auto new_w = grid_size.width() + 1;
            grid_size.setWidth(new_w);

            grid_layout->SetGridSize(grid_size);
            grid_layout->UpdateWidgetsToGridSize(grid_size);
        }else if(total_width > content_width){
            grid_size.setWidth(grid_size.width() - 2);
            grid_layout->SetGridSize(grid_size);
            grid_layout->UpdateWidgetsToGridSize(grid_size);
        }
    }

    grid_layout->update();
    this->m_AddIcon->OnParentSizeUpdate(m_Size);
    this->update();
}
