#ifndef SHADECIRCLE_HPP
#define SHADECIRCLE_HPP
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QComboBox>
#include <QKeyEvent>

class ShadeCircle : public QWidget{
    Q_OBJECT
private:
    QColor m_RelativeColor{QColor(55,55,255,255)};
    std::shared_ptr<QColor> m_SelectedShadeColor;
    int m_Radius{37};
    bool m_IsChoosingColor{false};
    int m_ShadesSteps{380};

    QPoint m_MousePosition;
    QPoint m_dragStart;
public:
    explicit ShadeCircle(QWidget* parent = Q_NULLPTR);
    ~ShadeCircle() Q_DECL_EQ_DEFAULT;

    void SetRelativeColor(QColor relative_color);
    QColor GetRelativeColor() const {return m_RelativeColor;}

    void SetShadeColor(std::shared_ptr<QColor> shade_color);
    std::shared_ptr<QColor> GetSelectedShade() const {return m_SelectedShadeColor;}
protected:
    void paintEvent(QPaintEvent * event) Q_DECL_OVERRIDE;

    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
signals:
    void ShadeColorChanged();
    void OnShadeChanged(std::shared_ptr<QColor> new_shade);
};

#endif // SHADECIRCLE_HPP
