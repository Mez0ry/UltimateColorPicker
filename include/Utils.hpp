#ifndef UTILS_HPP
#define UTILS_HPP
#include <QMainWindow>
#include <algorithm>
#include "Hsv.hpp"
#include <QApplication>
#include <vector>
#include <QJsonDocument>

namespace Utils{

    [[nodiscard]] QMainWindow* GetMainWindow();

    Hsv GetHsvFrom(const QPoint &point, float radius, Brightness brightness = Brightness(1.0f));

    QColor GetColorFrom(const QPoint& pt, QWidget* widget);
    QColor GetColorFrom(const QPoint& pt, const QPixmap& pixmap);

    QColor GetComplementaryFromFactor(QColor base_color, int hue_angle_factor = 180);
    std::vector<QColor> GetComplementary(QColor base_color);
    std::vector<QColor> GetTriadic(QColor base_color);
    std::vector<QColor> GetTetradic(QColor base_color);
    std::vector<QColor> GetAnalagous(QColor base_color);
    std::vector<QColor> GetSplitComplementary(QColor base_color);

    inline float CalculateLuminance(QColor color){
        return (float) (0.2126*color.red() + 0.7152* color.green() + 0.0722* color.blue());
    }

    inline QColor GetInveseredColor(QColor color) {
        float luminance = CalculateLuminance(color);
        QString inverse = (luminance < 140) ? "#fff" : "#000";

        QString base_name = color.name();
        base_name.replace(0,inverse.size(),inverse);

        QColor res(base_name);

        return res;
    }

    namespace Font{
        int GetAdaptiveFontSize(int desired_font_size, float standard_dpi = 96.0);
    }//!font

    namespace Json{
        QJsonDocument LoadJson(QString file_path);
        void SaveJson(QJsonDocument doc, QString file_path);

    } //!Json

}// !namespace

#endif // UTILS_HPP
