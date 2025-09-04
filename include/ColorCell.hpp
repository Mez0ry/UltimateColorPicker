#ifndef COLORCELL_HPP
#define COLORCELL_HPP
#include <QLabel>
#include <QColor>

class ColorCell : public QLabel{
    Q_OBJECT
private:
    QSize m_Size;
public:
    explicit ColorCell(QColor color = QColor(0,0,0,255),QSize size = {16,16},QWidget* parent = Q_NULLPTR);
    ~ColorCell() Q_DECL_EQ_DEFAULT;

    void SetColor(QColor color);
protected:
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
};

#endif // COLORCELL_HPP
