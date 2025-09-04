#include "mainwindow.h"
#include <QFile>
#include <QGraphicsOpacityEffect>
#include <QPointer>
#include <QPropertyAnimation>
#include <QSettings>
#include "ColorWheel.hpp"
#include "NavigationBar.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)  , m_Ui(new Ui::MainWindow)
{
    m_Ui->setupUi(this);
    this->centralWidget()->setMouseTracking(true);
    m_Ui->formLayout->addWidget(new ColorWheel());
    QPointer nav_bar = new NavigationBar(m_Ui->NavigationWidget);
    Q_UNUSED(nav_bar);

    QSettings settings("Mezory", "ColorPicker");

    settings.beginGroup("ConfigFiles");

    settings.setValue("color_pallete_config","data/color_palletes.json");

    settings.setValue("app_style_config",":/resources/style.qss");
    auto style_path = settings.value("app_style_config").toString();
    settings.endGroup();

    QFile style_file(style_path);
    if(!style_file.open(QFile::ReadOnly)){
        qCritical() << "failed to load style for application" << '\n';
    }

    QString style_str = QLatin1String(style_file.readAll());

    qApp->setStyleSheet(style_str);


    QGraphicsOpacityEffect *opacity_effect = new QGraphicsOpacityEffect(m_Ui->groupBox);
    m_Ui->groupBox->setGraphicsEffect(opacity_effect);

    QPropertyAnimation *opacity_anim = new QPropertyAnimation(opacity_effect,"opacity");
    opacity_anim->setDuration(1000);
    opacity_anim->setStartValue(0);
    opacity_anim->setEndValue(1);
    opacity_anim->setEasingCurve(QEasingCurve::InSine);
    opacity_anim->start(QPropertyAnimation::DeleteWhenStopped);
}

MainWindow::~MainWindow()
{
    delete m_Ui;
}
