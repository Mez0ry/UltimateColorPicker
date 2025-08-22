#ifndef COLOREDELLIPSE_H
#define COLOREDELLIPSE_H
#include <QWidget>

class ColoredEllipse : public QWidget{
    Q_OBJECT
private:
    QColor m_Color;
    QSize m_Size;
    int m_Radius;
public:
    ColoredEllipse(QColor color = Qt::white,int radius = 22, QWidget* parent = nullptr);
    ~ColoredEllipse() = default;

protected:
    void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
private:
};
#endif // COLOREDELLIPSE_H
