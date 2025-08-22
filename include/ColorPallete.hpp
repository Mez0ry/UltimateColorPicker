#ifndef COLORPALLETE_HPP
#define COLORPALLETE_HPP
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRadialGradient>
#include <QPainter>
#include <QLineEdit>
#include <QResizeEvent>
#include "Utils.hpp"
#include <QJsonObject>

#include <QSettings>
#include <QPainterPath>


class QVBoxLayout;

class DeletePopup;
class NamedPallete;
class ColoredEllipse;
class PalleteNameSpecifierPopup;
class AddIcon;

class QScrollArea;

class ColorPallete : public QWidget{
    Q_OBJECT
private:
    int m_Width,m_Height;
    QFrame* m_BackgroundVBoxLayout;
    QScrollArea* m_ScrollArea;
    AddIcon* m_AddIcon;
    QFrame* m_Content;
public:
    explicit ColorPallete(QWidget* parent = nullptr);
    ~ColorPallete();

    void OnButtonClicked();
protected:
    void moveEvent(QMoveEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
private:
    void OnResize(QSize new_size);
    void UpdateColorPalletes();

};

#endif // COLORPALLETE_HPP
