#pragma once
#include <QWidget>
#include <QColor>
#include <QPixmap>
#include <QPoint>

class QTimer;
class QPaintEvent;
class QMouseEvent;
class QKeyEvent;
class QEvent;

class ColorPicker : public QWidget {
    Q_OBJECT
private:
    QMetaObject::Connection m_TimerConnection;
    QTimer* m_Timer {Q_NULLPTR};
    QColor  m_SelectedColor {Qt::black};
    QPixmap m_MagnifierSrc;
    QPoint  m_CursorGlobal;
    int     m_Radius {42};
    int     m_Grid   {10};
    int m_Hole {32};
public:
    explicit ColorPicker(QWidget *parent = Q_NULLPTR);
    ~ColorPicker();

    void Start();
protected:
    void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent*) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent*) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent*) Q_DECL_OVERRIDE;
    bool event(QEvent*) Q_DECL_OVERRIDE;

    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
signals:
    void ColorPicked(QColor picked_color);
public slots:
    void OnButtonPush();
private:
    void sampleUnderCursor();
    QRect currentScreenLogicalGeometry() const;
};
