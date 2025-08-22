#ifndef NAVIGATIONBAR_H
#define NAVIGATIONBAR_H
#include <QWidget>
#include <QPushButton>
#include "ColorPicker.hpp"
#include "ColorCombo.hpp"
#include "ColorPallete.hpp"

class NavigationBar : public QWidget{
    Q_OBJECT
private:
public:
    explicit NavigationBar(QWidget* parent = nullptr);
    ~NavigationBar();

private:
    ColorPicker m_ColorPicker;
    ColorPallete m_ColorPallete;
};

#endif // NAVIGATIONBAR_H
