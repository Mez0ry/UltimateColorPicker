#include "Utils.hpp"
#include <QFile>
#include <QScreen>
#include <functional>
#include <QJsonObject>

QMainWindow *Utils::GetMainWindow()
{
    foreach (QWidget *w, qApp->topLevelWidgets())
    if (QMainWindow* main_win = qobject_cast<QMainWindow*>(w))
        return main_win;
    return nullptr;
}

QColor Utils::GetColorFrom(const QPoint &pt, QWidget *widget)
{
    QPixmap pixmap(widget->grab());
    auto img = pixmap.toImage();
    return img.pixelColor(pt);
}

QColor Utils::GetColorFrom(const QPoint &pt, const QPixmap &pixmap)
{
    auto img = pixmap.toImage();
    return img.pixelColor(pt);
}

QColor Utils::GetComplementaryFromFactor(QColor base_color, int hue_angle_factor)
{
    auto base_hue_angle = base_color.hslHue();

    auto complementary_angle = base_hue_angle + hue_angle_factor;
    if(complementary_angle > 360){
        complementary_angle -= 360;
    }else if(complementary_angle < 0){
        complementary_angle += 360;
    }

    QColor complementary{Qt::black};
    complementary.setHsv(complementary_angle,base_color.saturation(), base_color.value());

    return complementary;
}

std::vector<QColor> Utils::GetComplementary(QColor base_color)
{
    std::vector<QColor> res;
    res.push_back(base_color);
    res.push_back(GetComplementaryFromFactor(base_color));
    return res;
}

std::vector<QColor> Utils::GetTriadic(QColor base_color)
{
    std::vector<QColor> res;
    res.push_back(base_color);

    for(auto iterations_count = 0, angle_factor = 120; iterations_count < 2; iterations_count++, angle_factor *= 2){
        res.push_back(GetComplementaryFromFactor(base_color,angle_factor));
    }

    return res;
}

std::vector<QColor> Utils::GetTetradic(QColor base_color)
{
    std::vector<QColor> res;
    res.push_back(base_color);

    int angle[3] = {180, 90, 180};

    for(auto iterations_count = 0; iterations_count < 3; iterations_count++ ){
        res.push_back(GetComplementaryFromFactor((iterations_count != 0) ? res[iterations_count] : base_color,angle[iterations_count]));
    }

    return res;
}

std::vector<QColor> Utils::GetAnalagous(QColor base_color)
{
    std::vector<QColor> res;
    res.push_back(base_color);

    for(auto iteration = 0, p_hue_factor = 20, n_hue_factor = -20; iteration < 3; iteration++, p_hue_factor += 10, n_hue_factor -= 10){
        res.push_back(GetComplementaryFromFactor(base_color, p_hue_factor));
        res.push_back(GetComplementaryFromFactor(base_color, n_hue_factor));
    }

    return res;
}

std::vector<QColor> Utils::GetSplitComplementary(QColor base_color)
{
    std::vector<QColor> res;
    res.push_back(base_color);
    int hue_angles[2] = {150, 210};

    for(auto iterations_count = 0; iterations_count < 2; iterations_count++){
        res.push_back(GetComplementaryFromFactor(base_color, hue_angles[iterations_count]));
    }

    return res;
}

int Utils::Font::GetAdaptiveFontSize(int desired_font_size, float standard_dpi)
{
    QScreen* screen = QGuiApplication::primaryScreen();
    qreal dpi = screen->logicalDotsPerInch();
    return (desired_font_size * (dpi / standard_dpi));
}

QJsonDocument Utils::Json::LoadJson(QFile& file)
{
    if(!file.open(QFile::ReadOnly)){
        qCritical() << "failed to open a file, file name: " << file.fileName()  << '\n';
    }

    QJsonParseError parse_error;
    auto json_doc = QJsonDocument::fromJson(file.readAll(), &parse_error);

    if(parse_error.error != QJsonParseError::NoError){
        qCritical() << "failed to convert json text to QJsonDocument. Error: " << parse_error.errorString();
    }

    return json_doc;
}

void Utils::Json::SaveJson(QJsonDocument doc, QString file_path)
{
    QFile file(file_path);
    if(!file.open(QFile::WriteOnly)){
        qCritical() << "failed to open a file, file_path: " << file_path  << '\n';
    }

    file.write(doc.toJson());
}

QColor Utils::GetInveseredColorIf(QColor color, std::function<bool(int)> pred_luminance)
{
    float luminance = CalculateLuminance(color);
    QString inverse = (pred_luminance(luminance)) ? "#fff" : "#000";

    QString base_name = color.name();
    base_name.replace(0, inverse.size(), inverse);

    return QColor(base_name);
}

float Utils::CalculateLuminance(QColor color)
{
    return static_cast<float>(0.2126 * color.red() + 0.7152 * color.green() + 0.0722 * color.blue());
}

void Utils::Json::RemoveKeyFromJsonFileAndSave(const QString& json_file_path, const QString &key) {
    QFile file(json_file_path);
    auto json_doc = Utils::Json::LoadJson(file);
    auto root_obj = json_doc.object();
    root_obj.remove(key);
    json_doc.setObject(root_obj);

    Utils::Json::SaveJson(json_doc, json_file_path);

}
