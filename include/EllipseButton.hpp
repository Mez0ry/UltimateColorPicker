#ifndef ELLIPSEBUTTON_HPP
#define ELLIPSEBUTTON_HPP
#include <QPropertyAnimation>
#include <QPushButton>

class EllipseButton : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(qreal FillLevel READ GetFillLevel WRITE SetFillLevel NOTIFY FillLevelChanged)
private:
    QSize m_Size;
    const int m_Radius;
private:
    qreal m_FillLevel;
    QPropertyAnimation m_Animation;
    QColor m_FromFillColor{"#3C404E"}, m_ToFillColor{"#FE3377"};
public:
    EllipseButton(QWidget *parent = Q_NULLPTR, QSize size = QSize(16, 16));
    ~EllipseButton() Q_DECL_EQ_DEFAULT;

    qreal GetFillLevel() const { return m_FillLevel; }
    void SetFillLevel(qreal value);

    void StartAnimation(qreal targetLevel, int duration_ms);
signals:
    void FillLevelChanged(const qreal);
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // ELLIPSEBUTTON_HPP
