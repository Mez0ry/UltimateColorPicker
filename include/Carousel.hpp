#ifndef CAROUSEL_HPP
#define CAROUSEL_HPP
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEvent>

class ColorWheel;
class EllipseButton;
class QPropertyAnimation;

class Carousel : public QWidget {
    Q_OBJECT
private:
    int m_CurrentIndex;
    int m_PrevIndex;
    QVector<std::shared_ptr<ColorWheel>> m_Wheels;
    QVector<EllipseButton*> m_Buttons;
    QTimer* m_CarouselTimer;
    QVBoxLayout* m_MainLayout;
    QHBoxLayout* m_ButtonsLayout;
public:
    explicit Carousel(QWidget* parent = Q_NULLPTR);
    ~Carousel() Q_DECL_OVERRIDE;
protected:
    bool eventFilter(QObject* obj, QEvent* ev) Q_DECL_OVERRIDE;
private slots:
    void SelectItemAt(int index);
    void PreviousItem();
    void NextItem();
private:
    void ChangeCurrentItem();
};

#endif // CAROUSEL_HPP
