#ifndef COLORWHEEL_H
#define COLORWHEEL_H
#include <QWidget>

#include "ShadeCircle.hpp"
#include "ColorInfo.hpp"
#include "ColorCombo.hpp"

namespace internal{

class IColorWheel{
public:
    virtual ~IColorWheel(){}

    virtual void OnPaintEvent(QPainter& painter) = 0;
    virtual QColor GetColorAtPoint(QPoint point,float radius) const = 0;
private:
};

class OklabColorWheel : public IColorWheel{
private:
    double m_CurrentChroma = {1.0};
    double m_CurrentLightness = {1.0};
private:
    QRect m_ParentRect;
public:
    OklabColorWheel(QRect parent_rect);
    virtual ~OklabColorWheel();

    double GetChroma() const;
    void SetChroma(double chroma);

    double GetLightness() const;
    void SetLightness(double lightness);

    void OnPaintEvent(QPainter& painter) override;
    QColor GetColorAtPoint(QPoint point,float radius) const override;
private:
};

class HsvColorWheel : public IColorWheel{
private:
    double m_Brightness{1.0};
    double m_Saturation{1.0};
private:
    QRect m_ParentRect;
public:
    HsvColorWheel(QRect parent_rect);
    virtual ~HsvColorWheel();

    double GetSaturation() const;
    void SetSaturation(double saturation);

    double GetBrightness() const;
    void SetBrightness(double brightness);

    void OnPaintEvent(QPainter& painter) override;
    QColor GetColorAtPoint(QPoint point,float radius) const override;
private:
};

}//!internal

class ColorWheel : public QWidget{
    Q_OBJECT
    using hash_code_t = std::size_t;
private:
    const QSize m_Size{150,150};
    int m_Radius{m_Size.width() / 2};
    quint16 m_Margin{10};
    std::shared_ptr<QColor> m_SelectedColor{nullptr};

    std::unordered_map<size_t,std::shared_ptr<internal::IColorWheel>> m_ColorWheels;
    std::shared_ptr<internal::IColorWheel> m_CurrentColorWheel{nullptr};
private:
    ShadeCircle m_ShadeCircle;
    ColorInfo m_ColorInfo;
    ColorCombo m_ColorCombo;
    QPoint m_LastCursorPos;
public:
    explicit ColorWheel(QWidget *parent = Q_NULLPTR,QColor selected_color = QColor(255,255,255,255), quint16 margin = 10);
    ~ColorWheel() Q_DECL_EQ_DEFAULT;

    [[nodiscard]] std::shared_ptr<QColor>  GetSelectedColor() const {return m_SelectedColor;}
private:
    template <typename _ColorWheelType>
    void AddColorWheel(std::shared_ptr<_ColorWheelType> color_wheel);

    template <typename _ColorWheelType>
    inline void SetContext();

    template <typename _ColorWheelType>
    inline bool Contains();

    void OnPaintEvent(QPainter& painter);
    QColor GetColorAtPoint(QPoint point, float radius);
protected:
    void paintEvent(QPaintEvent * event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    void OnColorChange();
signals:
    void SigColorChanged();
};

template<typename _ColorWheelType>
inline bool ColorWheel::Contains()
{
    const hash_code_t hash = typeid(_ColorWheelType).hash_code();

    return (m_ColorWheels.count(hash) > 0);
}

template<typename _ColorWheelType>
inline void ColorWheel::SetContext()
{
    if (!Contains<_ColorWheelType>()) {
        throw std::runtime_error("Color wheel manager doesn't contain specified color wheel type");
    }

    const hash_code_t hash = typeid(_ColorWheelType).hash_code();

    m_CurrentColorWheel = m_ColorWheels[hash];
}

template<typename _ColorWheelType>
inline void ColorWheel::AddColorWheel(std::shared_ptr<_ColorWheelType> color_wheel)
{
    if (Contains<_ColorWheelType>()) {
        throw std::runtime_error("Color wheel already contain this type, its must be unique");
    }

    const hash_code_t hash = typeid(_ColorWheelType).hash_code();

    m_ColorWheels.insert(std::make_pair(hash, color_wheel));
}

#endif // COLORWHEEL_H
