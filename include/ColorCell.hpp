#ifndef COLORCELL_HPP
#define COLORCELL_HPP
#include <QWidget>
#include <QLabel>
#include <QColor>

class ColorCell : public QLabel{
    Q_OBJECT
private:
    int m_Height,m_Width;
public:
    explicit ColorCell(QColor color = QColor(0,0,0,255),QWidget* parent = nullptr);
    ~ColorCell() = default;

    void SetColor(QColor color);
protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif // COLORCELL_HPP
