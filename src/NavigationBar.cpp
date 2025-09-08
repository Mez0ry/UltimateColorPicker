#include "NavigationBar.hpp"
#include <QShortcut>
#include <QKeySequence>
#include <QDesktopServices>

#include "mainwindow.h"

#include "Utils.hpp"

NavigationBar::NavigationBar(QWidget *parent) : QWidget(parent)
{
    auto main_window = static_cast<MainWindow*>(Utils::GetMainWindow());
    auto ui = main_window->GetUi();

    ui->ColorPickerButton->setIcon(QIcon(":/resources/icon_color_picker_3.png"));
    ui->PalleteButton->setIcon(QIcon(":/resources/pallete_button.png"));
    ui->donation_button->setIcon(QIcon(":/resources/donation.png"));

    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+1"), parent);

    QObject::connect(shortcut, &QShortcut::activated,&m_ColorPicker, &ColorPicker::OnButtonPush);

    connect(ui->ColorPickerButton, &QPushButton::clicked, &m_ColorPicker, &ColorPicker::OnButtonPush);

    connect(ui->PalleteButton,&QPushButton::clicked,[=](bool pressed){
        Q_UNUSED(pressed);
        m_ColorPallete.OnButtonClicked();
    });

    connect(ui->donation_button,&QPushButton::clicked,[=](bool pressed){
        Q_UNUSED(pressed);
        QDesktopServices::openUrl(QUrl("https://www.donationalerts.com/r/hustrik1337", QUrl::TolerantMode));
    });
}
