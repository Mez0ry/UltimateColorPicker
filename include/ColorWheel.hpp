#ifndef COLORWHEEL_H
#define COLORWHEEL_H
#include <qwidget.h>
#include <QPainter>
#include <math.h>
#include <qmath.h>
#include <QMouseEvent>
#include <QTransform>
#include <QClipboard>
#include <QMimeData>
#include <QApplication>
#include <QLineEdit>
#include <QMainWindow>
#include <QStaticText>
#include <QPixmap>
#include <QMessageBox>
#include <QRegularExpression>
#include <QPushButton>
#include <QScreen>

#include "Hsv.hpp"
#include "Utils.hpp"
#include "mainwindow.h"

#include "./ui_mainwindow.h"
#include "ShadeCircle.hpp"
#include "ColorInfo.hpp"
#include "ColorCombo.hpp"

class ColorWheel : public QWidget{
    Q_OBJECT
private:
    quint16 m_Margin;
    float m_Radius;

    Hue m_Hue;
    Saturation m_Saturation;
    Brightness m_Brightness;

    std::shared_ptr<QColor> m_SelectedColor;
private:
    QLineF m_HueSelector;
    ShadeCircle m_ShadeCircle;
    ColorInfo m_ColorInfo;
    ColorCombo m_ColorCombo;// questionable here
public:
    explicit ColorWheel(QWidget *parent = nullptr,QColor selected_color = QColor(255,255,255,255), quint16 margin = 10);
    ~ColorWheel() Q_DECL_OVERRIDE;

    Hue GetHue() const {return m_Hue;}
    void SetHue(Hue hue){m_Hue = hue;}

    Saturation GetSaturation() const {return m_Saturation;}
    void SetSaturation(Saturation saturation){m_Saturation = saturation;}

    Brightness GetBrightness() const {return m_Brightness;}
    void SetBrightness(Brightness brightness){m_Brightness = brightness;}

protected:
    void paintEvent(QPaintEvent * event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    void OnColorChange();
signals:
    void SigColorChanged();
};

#endif // COLORWHEEL_H
