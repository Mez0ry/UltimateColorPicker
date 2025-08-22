#ifndef HSV_HPP
#define HSV_HPP
#include <QColor>

class Hue{
public:
    Hue() : m_Hue(0) {}

    explicit Hue(float hue_angle) : m_Hue(hue_angle){

    }
    ~Hue() = default;

    Hue(const Hue& other){
        this->m_Hue = other.m_Hue;
    }

    Hue(Hue&& other) : m_Hue(std::move(other.m_Hue)){

    }

    Hue& operator=(const Hue& other){
        this->m_Hue = other.m_Hue;
        return (*this);
    }

    Hue& operator=(Hue&& other) {
        this->m_Hue = std::move(other.m_Hue);
        return (*this);
    }

    Hue& operator=(float hue){
        qSwap(this->m_Hue,hue);
        return (*this);
    }

    void SetHue(float hue) { this->m_Hue = hue;}
    float GetHue() const {return this->m_Hue;}
private:
    void Clear(){ this->m_Hue = 0;}
private:
    float m_Hue;
};


class Saturation{
public:
    Saturation() : m_Saturation(0) {}
    Saturation(float saturation) : m_Saturation(saturation) {
        float min_saturation = 0;
        float max_saturation = 1;

        qBound(min_saturation,m_Saturation,max_saturation);
    }
    ~Saturation() = default;

    Saturation(const Saturation& other){
        this->m_Saturation = other.m_Saturation;
    }

    Saturation(Saturation&& other) : m_Saturation(std::move(other.m_Saturation)){

    }

    Saturation& operator=(const Saturation& other){
        this->m_Saturation = other.m_Saturation;
        return (*this);
    }

    Saturation& operator=(Saturation&& other) {
        this->m_Saturation = std::move(other.m_Saturation);
        return (*this);
    }

    void SetSaturation(float saturation) { m_Saturation = saturation;}
    float GetSaturation() const {return m_Saturation;}
private:
    float m_Saturation;
};


/**
 * @brief Strong type that represents 'brightness' or value in ColorWheel
*/
class Brightness{
public:
    Brightness() : m_Brightness(0) {}
    Brightness(float brightness) : m_Brightness(brightness) {


    }
    ~Brightness() = default;

    Brightness(const Brightness& other){
        this->m_Brightness = other.m_Brightness;
    }

    Brightness& operator=(Brightness other){
        qSwap(other.m_Brightness,this->m_Brightness);
        return (*this);
    }

    Brightness& operator=(float brightness){
        qSwap(this->m_Brightness,brightness);
        return (*this);
    }

    Brightness(Brightness&&) = delete;
    Brightness& operator=(Brightness&& other) = delete;

    void SetBrightness(float brightness) { m_Brightness = brightness;}
    float GetBrightness() const {return m_Brightness;}
private:
    float m_Brightness;
};

struct Hsv{
    Hsv() = default;
    Hsv(Hue h, Saturation s, Brightness b) : hue(h), saturation(s), brightness(b){}
    ~Hsv() = default;
    Hue hue;
    Saturation saturation;
    Brightness brightness;

    QColor GetAsColor() const {return QColor(hue.GetHue(),saturation.GetSaturation(),brightness.GetBrightness());}
};
#endif // HSV_HPP
