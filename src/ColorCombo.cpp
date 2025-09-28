#include "ColorCombo.hpp"
#include <QComboBox>
#include <QMainWindow>
#include <QGridLayout>
#include <QProxyStyle>

#include "mainwindow.h"

#include "ColorCell.hpp"
#include "Utils.hpp"

ColorCombo::ColorCombo(QWidget *parent) : QWidget(parent){
    auto main_window = static_cast<MainWindow*>(Utils::GetMainWindow());
    auto ui = main_window->GetUi();

    this->m_ComboBox = ui->ColorCombsBox;
    this->m_ColorComboGrid = ui->ColorCombosGrid;

    m_ComboBox->setDuplicatesEnabled(false);
    m_ComboBox->addItem("Complementary");
    m_ComboBox->addItem("Split complementary");
    m_ComboBox->addItem("Triadic");
    m_ComboBox->addItem("Tetradic");
    m_ComboBox->addItem("Analogous");

    connect(this, &ColorCombo::BaseColorChanged, this, [=](){
        this->UpdateColor();
    });

    connect(m_ComboBox, &QComboBox::currentIndexChanged, this, [=](int new_index){
        m_CurrentComboIndex = new_index;
        ClearGridLayout(m_ColorComboGrid);
        this->UpdateColor();
    });
}

void ColorCombo::SetBaseColor(QColor *base_color)
{
    m_ColorTypesMap.clear();

    this->m_BaseColor = base_color;

    m_ColorTypesMap.insert(std::make_pair(static_cast<int>(ColorType::COMPLEMENTARY),Utils::GetComplementary(QColor(*base_color))));
    m_ColorTypesMap.insert(std::make_pair(static_cast<int>(ColorType::TETRADIC),Utils::GetTetradic(QColor(*base_color))));
    m_ColorTypesMap.insert(std::make_pair(static_cast<int>(ColorType::TRIADIC),Utils::GetTriadic(QColor(*base_color))));
    m_ColorTypesMap.insert(std::make_pair(static_cast<int>(ColorType::ANALOGOUS),Utils::GetAnalagous(QColor(*base_color))));
    m_ColorTypesMap.insert(std::make_pair(static_cast<int>(ColorType::SPLIT_COMPLEMENTARY),Utils::GetSplitComplementary(QColor(*base_color))));

    emit BaseColorChanged();
}

void ColorCombo::UpdateColor()
{
    if(!m_ColorTypesMap.count(m_CurrentComboIndex)){
        return;
    }

    std::vector<QColor> color_type = m_ColorTypesMap[m_CurrentComboIndex];
    ClearGridLayout(m_ColorComboGrid);
    AddColorCellsToLayout(m_ColorComboGrid, color_type);
    this->update();

}

void ColorCombo::ClearGridLayout(QGridLayout *layout)
{
    while(layout->count()) {
        QWidget* widget = layout->itemAt(0)->widget();
        if( widget ) {
            layout->removeWidget(widget);
            delete widget;
        }
    }
}

void ColorCombo::AddColorCellsToLayout(QGridLayout *layout, std::vector<QColor> &colors)
{
    QPoint pos(0,0);

    for(auto& color : colors){
        if((pos.x() % 4) == 0){
            pos.setY(pos.y() + 1);
            pos.setX(0);
        }

        layout->addWidget(new ColorCell(color),pos.y(),pos.x());

        pos.setX(pos.x() + 1);
    }

    layout->update();
}
