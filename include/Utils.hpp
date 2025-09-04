#ifndef UTILS_HPP
#define UTILS_HPP
#include <QMainWindow>
#include <QApplication>
#include <vector>
#include <QJsonDocument>

namespace Utils{

    [[nodiscard]] QMainWindow* GetMainWindow();

    QColor GetColorFrom(const QPoint& pt, QWidget* widget);
    QColor GetColorFrom(const QPoint& pt, const QPixmap& pixmap);

    QColor GetComplementaryFromFactor(QColor base_color, int hue_angle_factor = 180);
    std::vector<QColor> GetComplementary(QColor base_color);
    std::vector<QColor> GetTriadic(QColor base_color);
    std::vector<QColor> GetTetradic(QColor base_color);
    std::vector<QColor> GetAnalagous(QColor base_color);
    std::vector<QColor> GetSplitComplementary(QColor base_color);

    float CalculateLuminance(QColor color);

    QColor GetInveseredColor(QColor color, int inverse_if_less_than_val);

    namespace Font{
        int GetAdaptiveFontSize(int desired_font_size, float standard_dpi = 96.0);
    }//!font

    namespace Json{
        QJsonDocument LoadJson(QString file_path);
        void SaveJson(QJsonDocument doc, QString file_path);

    } //!Json

}// !namespace

#endif // UTILS_HPP
