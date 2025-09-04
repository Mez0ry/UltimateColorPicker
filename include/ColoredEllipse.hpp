#ifndef COLOREDELLIPSE_HPP
#define COLOREDELLIPSE_HPP
#include <QWidget>

class ColoredEllipse : public QWidget{
    Q_OBJECT
private:
    QColor m_Color;
    QSize m_Size;
    int m_Radius;
public:
    ColoredEllipse(QColor color = Qt::white,int radius = 22, QWidget* parent = Q_NULLPTR);
    ~ColoredEllipse() Q_DECL_EQ_DEFAULT;

protected:
    void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
private:
};
#endif // COLOREDELLIPSE_HPP
