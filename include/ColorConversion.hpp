#ifndef COLORCONVERSION_HPP
#define COLORCONVERSION_HPP
#include <QColor>
#include <QMatrix4x4>
#include <QTransform>
#include <QVector>
#include <QtGlobal>
#include <cstdint>
#include <QGenericMatrix>

namespace Utils::ColorConversion{
namespace internal{
    using color_conversion_mat_4v4_t = QGenericMatrix<4, 4, float>;

    class ConversionMatricies
    {
    public:
        ConversionMatricies(const ConversionMatricies&) = delete;
        ConversionMatricies(ConversionMatricies&&) = delete;
        ConversionMatricies& operator=(const ConversionMatricies&) = delete;
        ConversionMatricies& operator=(ConversionMatricies&&) = delete;

        static inline const QMatrix4x4& rgb_to_xyz_mat() {
            constexpr float rgb_to_xyz_values[] = {
                0.4124564, 0.3575761, 0.1804375, 0,
                0.2126729, 0.7151522, 0.0721750, 0,
                0.0193339, 0.1191920, 0.9503041, 0,
                0, 0, 0, 1
            };

            static const color_conversion_mat_4v4_t rgbxyz_mat(rgb_to_xyz_values);
            static const QMatrix4x4 mat(rgbxyz_mat);

            return mat;
        }

        static inline const QMatrix4x4& xyz_to_rgb_mat() {
            constexpr float xyz_to_rgb_values[] = {
                3.2404542, -1.5371385, -0.4985314, 0,
                -0.9692660, 1.8760108, 0.0415560, 0,
                0.0556434, -0.2040259, 1.0572252, 0,
                0, 0, 0, 1
            };

            static const color_conversion_mat_4v4_t xyz_to_rgb(xyz_to_rgb_values);
            static const QMatrix4x4 mat(xyz_to_rgb);
            return mat;
        }

    private:
        ConversionMatricies() = delete;
    };

    inline QVector3D get_normalized_rgb(const QRgb& rgb);

    inline float apply_gamma_encode(const float x);
    void apply_gamma_encode(QVector<qreal>& vec);
    void apply_gamma_encode(QVector3D& vec);

    inline float apply_gamma_decode(const float x);
    void apply_gamma_decode(QVector<qreal>& vec);
    void apply_gamma_decode(QVector3D& normalized_rgb);
} //!internal

inline float normalize_component(uint8_t component);
inline QVector3D rgb_to_u8_range(QVector3D srgb);

QVector3D rgb_to_xyz(const QRgb &rgb, const float scale_factor = 100);
QRgb xyz_to_rgb(QVector3D xyz, const float unscale_factor = 100);

/*
* @return scaled xyz
*/
inline QVector3D oklab_to_xyz(double L, double a, double b);
/*
 * @return QColor as rgb color
*/
QColor oklab_to_rgb(double L, double a, double b);

} //!namespace
#endif // COLORCONVERSION_HPP
