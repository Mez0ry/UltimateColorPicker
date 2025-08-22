#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "ColorWheel.hpp"
#include "NavigationBar.hpp"
#include <QMdiSubWindow>
#include <QMdiArea>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    [[nodiscard]] Ui::MainWindow* GetUi(){
        return m_Ui;
    }

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE{
        Q_UNUSED(event);
        this->setFocus();
    }
private:
    Ui::MainWindow *m_Ui;
};

#endif // MAINWINDOW_H
