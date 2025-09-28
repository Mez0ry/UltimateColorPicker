#ifndef COLORWHEEL_H
#define COLORWHEEL_H
#include <QWidget>
#include <QMetaType>
#include <mutex>

#include "ShadeCircle.hpp"

class ColorWheel : public QWidget{
    Q_OBJECT
    Q_PROPERTY(qreal ScaleFactor READ GetScaleFactor WRITE SetScaleFactor NOTIFY ScaleFactorChanged)
protected:
    const QSize m_Size{150, 150};
    int m_Radius{m_Size.width() / 2};
private:
    quint16 m_Margin{10};
    std::shared_ptr<QColor> m_SelectedColor{Q_NULLPTR};
    qreal m_ScaleFactor{1.0};
private:
    ShadeCircle m_ShadeCircle;
    QPoint m_LastCursorPos;
    inline static std::once_flag m_InitOnceFlag{};
public:
    explicit ColorWheel(QWidget *parent = Q_NULLPTR, QColor selected_color = QColor(255,255,255,255), quint16 margin = 10);
    virtual ~ColorWheel() Q_DECL_EQ_DEFAULT;

    ShadeCircle& GetShadeCircle() {return m_ShadeCircle;}

    qreal GetScaleFactor() const {return m_ScaleFactor;}
    void SetScaleFactor(qreal scale_factor) {m_ScaleFactor = scale_factor; update();}
protected:
    virtual void OnPaintEvent(QPainter& painter) = 0;
    virtual QColor GetColorAtPoint(QPoint point, float radius) const = 0;
protected:
    void paintEvent(QPaintEvent * event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    void OnColorChange();
signals:
    void ColorChanged();
    void ScaleFactorChanged();
};

class OklabColorWheel : public ColorWheel{
    Q_OBJECT
private:
    double m_CurrentChroma = {1.0};
    double m_CurrentLightness = {1.0};
private:
    QRect m_ParentRect;
public:
    OklabColorWheel(QWidget* parent);
    ~OklabColorWheel() Q_DECL_EQ_DEFAULT;

    double GetChroma() const;
    void SetChroma(double chroma);

    double GetLightness() const;
    void SetLightness(double lightness);

    void OnPaintEvent(QPainter& painter) override;
    QColor GetColorAtPoint(QPoint point, float radius) const override;
private:
};

class HsvColorWheel : public ColorWheel{
    Q_OBJECT
private:
    double m_Brightness{1.0};
    double m_Saturation{1.0};
private:
    QRect m_ParentRect;
public:
    HsvColorWheel(QWidget* parent);
    ~HsvColorWheel() Q_DECL_EQ_DEFAULT;

    double GetSaturation() const;
    void SetSaturation(double saturation);

    double GetBrightness() const;
    void SetBrightness(double brightness);

    void OnPaintEvent(QPainter& painter) override;
    QColor GetColorAtPoint(QPoint point, float radius) const override;
private:
};
#endif // COLORWHEEL_H
