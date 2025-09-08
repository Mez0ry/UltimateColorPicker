#include "ColorConversion.hpp"

float Utils::ColorConversion::normalize_component(uint8_t component)
{
    return qBound(0.0f, component / 255.0f, 1.0f);
}

QVector3D Utils::ColorConversion::internal::get_normalized_rgb(const QRgb &rgb)
{
    return QVector3D(normalize_component(qRed(rgb)),
                     normalize_component(qGreen(rgb)),
                     normalize_component(qBlue(rgb)));
}

float Utils::ColorConversion::internal::apply_gamma_encode(const float x)
{
    return (x <= 0.0031308) ? 12.92 * x : 1.055 * std::pow(x, 1.0 / 2.4) - 0.055;
}

void Utils::ColorConversion::internal::apply_gamma_encode(QVector<qreal> &vec)
{
    for (auto &v : vec) {
        v = apply_gamma_encode(v);
    }
}

void Utils::ColorConversion::internal::apply_gamma_encode(QVector3D &vec)
{
    vec.setX(apply_gamma_encode(vec.x()));
    vec.setY(apply_gamma_encode(vec.y()));
    vec.setZ(apply_gamma_encode(vec.z()));
}

float Utils::ColorConversion::internal::apply_gamma_decode(const float x)
{
    return (x > 0.04045) ? std::pow((x + 0.055) / 1.055, 2.4) : x / 12.92;
}

void Utils::ColorConversion::internal::apply_gamma_decode(QVector<qreal> &vec)
{
    for (auto &v : vec) {
        v = apply_gamma_decode(v);
    }
}

void Utils::ColorConversion::internal::apply_gamma_decode(QVector3D &normalized_rgb)
{
    normalized_rgb.setX(apply_gamma_decode(normalized_rgb.x()));
    normalized_rgb.setY(apply_gamma_decode(normalized_rgb.y()));
    normalized_rgb.setZ(apply_gamma_decode(normalized_rgb.z()));
}

QVector3D Utils::ColorConversion::rgb_to_xyz(const QRgb &rgb)
{
    const int scale_factor = 100;

    QVector3D normalized_rgb(internal::get_normalized_rgb(rgb));
    internal::apply_gamma_decode(normalized_rgb);

    return internal::ConversionMatricies::rgb_to_xyz_mat().map(normalized_rgb * scale_factor);
}

QVector3D Utils::ColorConversion::rgb_to_u8_range(QVector3D srgb)
{
    return QVector3D(std::round(std::clamp((float) (srgb.x() * 255.0), 0.0f, 255.0f)),
                     std::round(std::clamp((float) (srgb.y() * 255.0), 0.0f, 255.0f)),
                     std::round(std::clamp((float) (srgb.z() * 255.0), 0.0f, 255.0f)));
}

QRgb Utils::ColorConversion::xyz_to_rgb(QVector3D xyz)
{

    QVector3D srgb = internal::ConversionMatricies::xyz_to_rgb_mat().map(xyz / 100);
    internal::apply_gamma_encode(srgb);
    srgb = rgb_to_u8_range(srgb);

    return qRgb(srgb.x(), srgb.y(), srgb.z());
}

QColor Utils::ColorConversion::oklab_to_rgb(double L, double a, double b)
{
    const int scale_factor = 100;
    return QColor::fromRgb(xyz_to_rgb(oklab_to_xyz(L,a,b) * scale_factor));
}

QVector3D Utils::ColorConversion::oklab_to_xyz(double L, double a, double b) {

    const float root_l = sqrt(L);
    const float m_prime = L + 1.5 * root_l * a;
    const float s_prime = L + 1.5 * root_l * b;

    const float y_p = L * L;
    const float x_p = y_p * m_prime;
    const float z_p = y_p * s_prime;

    const float x_w = 0.95047, y_w = 1.0, z_w = 1.08883;

    return QVector3D{x_p * x_w, y_p * y_w, z_p * z_w};
}
