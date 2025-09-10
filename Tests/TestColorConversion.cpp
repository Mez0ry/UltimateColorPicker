#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "ColorConversion.hpp"

TEST_CASE("Testing rgb normalization function in ColorConversion", "[ColorConversion][Normalization]")
{
    SECTION("Normalize Minimum Values (all zeros)")
    {
        auto input = QColor{0, 0, 0}.rgb();
        QVector3D expected_result(0, 0, 0);

        REQUIRE(Utils::ColorConversion::internal::get_normalized_rgb(input).x() == Catch::Approx(expected_result.x()).epsilon(0.001));
        REQUIRE(Utils::ColorConversion::internal::get_normalized_rgb(input).y() == Catch::Approx(expected_result.y()).epsilon(0.001));
        REQUIRE(Utils::ColorConversion::internal::get_normalized_rgb(input).z() == Catch::Approx(expected_result.z()).epsilon(0.001));
    }

    SECTION("Normalize Maximum Values (all ones)")
    {
        auto input = QColor{255, 255, 255}.rgb();
        QVector3D expected_result(1.f, 1.f, 1.f);

        REQUIRE(Utils::ColorConversion::internal::get_normalized_rgb(input).x() == Catch::Approx(expected_result.x()).epsilon(0.001));
        REQUIRE(Utils::ColorConversion::internal::get_normalized_rgb(input).y() == Catch::Approx(expected_result.y()).epsilon(0.001));
        REQUIRE(Utils::ColorConversion::internal::get_normalized_rgb(input).z() == Catch::Approx(expected_result.z()).epsilon(0.001));
    }

    SECTION("Normalize MidRange values")
    {
        auto input = QColor{128, 128, 128}.rgb();
        QVector3D expected_result(0.5f, 0.5f, 0.5f);

        REQUIRE(Utils::ColorConversion::internal::get_normalized_rgb(input).x() == Catch::Approx(expected_result.x()).epsilon(0.005));
        REQUIRE(Utils::ColorConversion::internal::get_normalized_rgb(input).y() == Catch::Approx(expected_result.y()).epsilon(0.005));
        REQUIRE(Utils::ColorConversion::internal::get_normalized_rgb(input).z() == Catch::Approx(expected_result.z()).epsilon(0.005));
    }

    SECTION("Normalize close to boundaries")
    {
        auto input = QColor{1, 254, 127}.rgb();
        QVector3D expected_result((float)(1)/255, (float)(254)/255, (float)(127)/255);

        REQUIRE(Utils::ColorConversion::internal::get_normalized_rgb(input).x() == Catch::Approx(expected_result.x()).epsilon(0.001));
        REQUIRE(Utils::ColorConversion::internal::get_normalized_rgb(input).y() == Catch::Approx(expected_result.y()).epsilon(0.001));
        REQUIRE(Utils::ColorConversion::internal::get_normalized_rgb(input).z() == Catch::Approx(expected_result.z()).epsilon(0.001));
    }

    SECTION("Float precision check (tolerance-based comparison)")
    {
        auto input = QColor{128, 128, 128}.rgb();
        QVector3D expected_result(0.5f,0.5f,0.5f);

        REQUIRE(Utils::ColorConversion::internal::get_normalized_rgb(input).x() == Catch::Approx(expected_result.x()).epsilon(0.005));
        REQUIRE(Utils::ColorConversion::internal::get_normalized_rgb(input).y() == Catch::Approx(expected_result.y()).epsilon(0.005));
        REQUIRE(Utils::ColorConversion::internal::get_normalized_rgb(input).z() == Catch::Approx(expected_result.z()).epsilon(0.005));
    }

    SECTION("Normalize component function ( Precion check )"){
        constexpr uint8_t input = 128;
        constexpr float expected_result = 0.5f;
        REQUIRE(Utils::ColorConversion::normalize_component(input) == Catch::Approx(expected_result).epsilon(0.005));
    }
}

TEST_CASE("Testing conversion from normalized to u8 range in ColorConversion", "[ColorConversion][ToU8Range]"){

    SECTION("All in one check"){
        constexpr int size = 15;

        std::array<QVector3D, size> input_arr = {{
            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f},
            {0.75f, 0.75f, 0.75f},
            {1.0f, 1.0f, 0.0f},
            {0.5f, 0.0f, 0.0f},
            {0.18431372549019607f, 0.5450980392156862f, 0.3411764705882353f},
            {0.0f, 1.0f, 1.0f},
            {0.5f, 0.0f, 0.5f},
            {0.5f, 0.5f, 0.5f},
            {0.0f, 0.1f, 0.0f},
            {0.5f, 1.0f, 0.0f},
            {0.5294117647058824f, 0.807843137254902f, 0.9215686274509803f}
        }};

        std::array<QVector3D, size> expected_arr = {{
            {0, 0, 0},
            {255, 255, 255},
            {255, 0, 0},
            {0, 255, 0},
            {0, 0, 255},
            {191, 191, 191},
            {255, 255, 0},
            {128, 0, 0},
            {47, 139, 87},
            {0, 255, 255},
            {128, 0, 128},
            {128, 128, 128},
            {0, 26, 0},
            {128, 255, 0},
            {135, 206, 235}
        }};

        for(int i = 0; i < size;i++){
            auto converted = Utils::ColorConversion::rgb_to_u8_range(input_arr[i]);

            REQUIRE(converted.x() == Catch::Approx(expected_arr[i].x()).epsilon(0.005));
            REQUIRE(converted.y() == Catch::Approx(expected_arr[i].y()).epsilon(0.005));
            REQUIRE(converted.z() == Catch::Approx(expected_arr[i].z()).epsilon(0.005));
        }
    }
}

TEST_CASE("Testing gamma encoding function in ColorConversion", "[ColorConversion][GammaEncoding]"){
    SECTION("Encode Minimum Value (0)")
    {
        constexpr float input = 0.0f;

        REQUIRE(Utils::ColorConversion::internal::apply_gamma_encode(input) == 0.0f);
    }

    SECTION("Encode Mid Value (~0.5)")
    {
        constexpr float input = 0.5f;
        constexpr float expected_value = 0.7354;

        REQUIRE(Utils::ColorConversion::internal::apply_gamma_encode(input) == Catch::Approx(expected_value).epsilon(0.01));
    }

    SECTION("Encode Maximum Value (1)")
    {
        constexpr float input = 1.0f;

        REQUIRE(Utils::ColorConversion::internal::apply_gamma_encode(input) == 1.0f);
    }

    SECTION("Encode Small Positive Value Near Zero (0.001f)")
    {
        constexpr float input = 0.001f;
        constexpr float expected_value = 0.01312;

        REQUIRE(Utils::ColorConversion::internal::apply_gamma_encode(input) == Catch::Approx(expected_value).epsilon(0.02));
    }

    SECTION("Encode QVector3d(5.61993, -0.381887, 1.07933)"){
        QVector3D input{5.61993,-0.381887,1.07933};
        Utils::ColorConversion::internal::apply_gamma_encode(input);

        constexpr int size = 3;
        std::array<double,size> arr = {input.x(),input.y(),input.z()};
        constexpr std::array<double,size> expected_arr = {2.11091f, -4.93399f, 1.0341f};

        for(int i = 0; i < size; i++){
            REQUIRE(arr[i] == Catch::Approx(expected_arr[i]).epsilon(0.02));
        }
    }
}

TEST_CASE("Testing gamma decoding function in ColorConversion","[ColorConversion][GammaDecoding]"){
    SECTION( "Decode Minimum Value (0)" ) {
        constexpr float input = 0;
        REQUIRE(Utils::ColorConversion::internal::apply_gamma_decode(input) == 0);
    }

    SECTION( "Decode High Mid Value (128)" ) {
        constexpr float input = 128;
        constexpr float expected_value = 7.385728836f;

        REQUIRE(Utils::ColorConversion::internal::apply_gamma_decode(input) == Catch::Approx(expected_value).epsilon(0.001));
    }

    SECTION( "Decode Mid Value (127.5)" ) {
        constexpr float input = 127.5f;
        constexpr float expected_value = 7.373699188f;

        REQUIRE(Utils::ColorConversion::internal::apply_gamma_decode(input) == Catch::Approx(expected_value).epsilon(0.001));
    }

    SECTION( "Decode Low Mid Value (127.5)" ) {
        constexpr float input = 127;
        constexpr float expected_value = 7.361641884f;

        REQUIRE(Utils::ColorConversion::internal::apply_gamma_decode(input) == Catch::Approx(expected_value).epsilon(0.001));
    }

    SECTION( "Decode Maximum Value (255) ") {
        constexpr float input = 255;
        constexpr float expected_value =  9.841823578f;

        REQUIRE(Utils::ColorConversion::internal::apply_gamma_decode(input) == Catch::Approx(expected_value).epsilon(0.001));
    }

    SECTION( "Decode Near Low Boundary Value (1)" ) {
        constexpr float input = 1;
        REQUIRE(Utils::ColorConversion::internal::apply_gamma_decode(input) == 1);
    }

    SECTION( "Decode Near Low Boundary Value (254)" ) {
        constexpr float input = 254;
        constexpr float expected_value = 9.825726509f;

        REQUIRE(Utils::ColorConversion::internal::apply_gamma_decode(input) == Catch::Approx(expected_value).epsilon(0.001));
    }

    SECTION( "Decode Negative data (-1)" ) {
        constexpr float invalid_input = -1;
        REQUIRE_THROWS_AS(Utils::ColorConversion::internal::apply_gamma_decode(invalid_input), std::invalid_argument);
    }

    SECTION("Decode QVector3d(127, 127, 127)"){
        QVector3D input{127,127,127};
        Utils::ColorConversion::internal::apply_gamma_decode(input);

        constexpr int size = 3;
        std::array<double,size> arr = {input.x(),input.y(),input.z()};
        constexpr std::array<double,size> expected_arr = {7.36164188385009766, 7.36164188385009766, 7.36164188385009766};

        for(int i = 0; i < size; i++){
            REQUIRE(arr[i] == Catch::Approx(expected_arr[i]).epsilon(0.001));
        }
    }
}

TEST_CASE("Testing from RGB To XYZ conversion function in ColorConversion","[ColorConversion][RGB_TO_XYZ]"){

    SECTION("Convert RGB (0, 0, 0)"){
        auto input = QColor(0, 0, 0).rgb();
        auto xyz = Utils::ColorConversion::rgb_to_xyz(input);

        REQUIRE(xyz.x() == Catch::Approx(0.0f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.0f).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.0f).epsilon(0.001));
    }

    SECTION("Convert RGB (255, 255, 255)"){
        auto input = QColor(255, 255, 255).rgb();
        auto xyz = Utils::ColorConversion::rgb_to_xyz(input);

        REQUIRE(xyz.x() == Catch::Approx(95.047f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(100.0f).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(108.883f).epsilon(0.001));
    }

    SECTION("Convert RGB (255, 0, 0)"){
        auto input = QColor(255, 0, 0).rgb();
        auto xyz = Utils::ColorConversion::rgb_to_xyz(input);

        REQUIRE(xyz.x() == Catch::Approx(41.2456f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(21.2673f).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(1.9334f).epsilon(0.001));
    }

    SECTION("Convert RGB (0, 255, 0)"){
        auto input = QColor(0, 255, 0).rgb();
        auto xyz = Utils::ColorConversion::rgb_to_xyz(input);

        REQUIRE(xyz.x() == Catch::Approx(35.7576f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(71.5152f).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(11.9192f).epsilon(0.001));
    }

    SECTION("Convert RGB (0, 0, 255)"){
        auto input = QColor(0, 0, 255).rgb();
        auto xyz = Utils::ColorConversion::rgb_to_xyz(input);

        REQUIRE(xyz.x() == Catch::Approx(18.0437f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(7.2175f).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(95.0304f).epsilon(0.001));
    }

    SECTION("Convert RGB (128, 128, 128)"){
        auto input = QColor(128, 128, 128).rgb();
        auto xyz = Utils::ColorConversion::rgb_to_xyz(input);

        REQUIRE(xyz.x() == Catch::Approx(72.83556366f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(76.631118774f).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(83.438247681f).epsilon(0.001));
    }

    SECTION("Convert RGB (255, 255, 0)"){
        auto input = QColor(255, 255, 0).rgb();
        auto xyz = Utils::ColorConversion::rgb_to_xyz(input);

        REQUIRE(xyz.x() == Catch::Approx(76.9984f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(92.7825f).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(13.8526f).epsilon(0.001));
    }

    SECTION("Convert RGB (0, 255, 255)"){
        auto input = QColor(0, 255, 255).rgb();
        auto xyz = Utils::ColorConversion::rgb_to_xyz(input);

        REQUIRE(xyz.x() == Catch::Approx(53.8013f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(78.7327f).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(106.9496f).epsilon(0.001));
    }

    SECTION("Convert RGB (255, 0, 255)"){
        auto input = QColor(255, 0, 255).rgb();
        auto xyz = Utils::ColorConversion::rgb_to_xyz(input);

        REQUIRE(xyz.x() == Catch::Approx(59.28935f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(28.4848f).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(96.9638f).epsilon(0.001));
    }

    SECTION("Convert RGB (47, 138, 87)"){
        auto input = QColor(47, 138, 87).rgb();
        auto xyz = Utils::ColorConversion::rgb_to_xyz(input);

        REQUIRE(xyz.x() == Catch::Approx(62.416267395f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx( 72.639846802f).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(73.625465393f).epsilon(0.001));
    }

    SECTION("Convert RGB (135, 206, 235)"){
        auto input = QColor(135, 206, 235).rgb();
        auto xyz = Utils::ColorConversion::rgb_to_xyz(input);

        REQUIRE(xyz.x() == Catch::Approx(82.603210449f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(89.384284973f).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(104.49319458f).epsilon(0.001));
    }

    SECTION("Convert RGB (128, 0, 0)"){
        auto input = QColor(128, 0, 0).rgb();
        auto xyz = Utils::ColorConversion::rgb_to_xyz(input);

        REQUIRE(xyz.x() == Catch::Approx(31.606988907f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(16.297359467f).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(1.481578112f).epsilon(0.001));
    }

    SECTION("Convert RGB (0, 128, 0)"){
        auto input = QColor(0, 128, 0).rgb();
        auto xyz = Utils::ColorConversion::rgb_to_xyz(input);

        REQUIRE(xyz.x() == Catch::Approx(27.401451111f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(54.802902222f).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(9.133813858f).epsilon(0.001));
    }

    SECTION("Convert RGB (0, 0, 128)"){
        auto input = QColor(0, 0, 128).rgb();
        auto xyz = Utils::ColorConversion::rgb_to_xyz(input);

        REQUIRE(xyz.x() == Catch::Approx(13.827125549f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(5.530849934f).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(72.822853088f).epsilon(0.001));
    }

    SECTION("Convert RGB (128, 128, 0)"){
        auto input = QColor(128, 128, 0).rgb();
        auto xyz = Utils::ColorConversion::rgb_to_xyz(input);

        REQUIRE(xyz.x() == Catch::Approx(59.00843811f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(71.100265503f).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(10.615391731f).epsilon(0.001));
    }

    SECTION("Convert RGB (128, 0, 128)"){
        auto input = QColor(128, 0, 128).rgb();
        auto xyz = Utils::ColorConversion::rgb_to_xyz(input);

        REQUIRE(xyz.x() == Catch::Approx(45.434112549f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(21.828208923f).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(74.304428101f).epsilon(0.001));
    }

    SECTION("Convert RGB (0, 128, 128)"){
        auto input = QColor(0, 128, 128).rgb();
        auto xyz = Utils::ColorConversion::rgb_to_xyz(input);

        REQUIRE(xyz.x() == Catch::Approx(41.22857666f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(60.333751678f).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(81.956665039f).epsilon(0.001));
    }
}

TEST_CASE("Testing from XYZ (xyz represented in percantage) To RGB conversion function in ColorConversion","[ColorConversion][XYZ_TO_RGB]"){
    SECTION("Convert XYZ (0.0, 0.0, 0.0)"){
        QVector3D input = {0.0, 0.0, 0.0};
        auto rgb = QColor(Utils::ColorConversion::xyz_to_rgb(input, 100));

        REQUIRE(rgb.red() == Catch::Approx(0).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(0).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(0).epsilon(0.001));
    }

    SECTION("Convert XYZ (95.047, 100.0, 108.883)"){
        QVector3D input = {95.047, 100.0, 108.883};
        auto rgb = QColor(Utils::ColorConversion::xyz_to_rgb(input, 100));

        REQUIRE(rgb.red() == Catch::Approx(255).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(255).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(255).epsilon(0.001));
    }

    SECTION("Convert XYZ (41.2456, 21.2673, 1.9334)"){
        QVector3D input = {41.2456, 21.2673, 1.9334};
        auto rgb = QColor(Utils::ColorConversion::xyz_to_rgb(input, 100));

        REQUIRE(rgb.red() == Catch::Approx(255).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(0).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(0).epsilon(0.001));
    }

    SECTION("Convert XYZ (35.7576, 71.5152, 11.9192)"){
        QVector3D input = {35.7576, 71.5152, 11.9192};
        auto rgb = QColor(Utils::ColorConversion::xyz_to_rgb(input, 100));

        REQUIRE(rgb.red() == Catch::Approx(0).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(255).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(0).epsilon(0.001));
    }

    SECTION("Convert XYZ (18.0437, 7.2175, 95.0304)"){
        QVector3D input = {18.0437, 7.2175, 95.0304};
        auto rgb = QColor(Utils::ColorConversion::xyz_to_rgb(input, 100));

        REQUIRE(rgb.red() == Catch::Approx(0).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(0).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(255).epsilon(0.001));
    }

    SECTION("Convert XYZ (50.0, 50.0, 50.0)"){
        QVector3D input = {50.0, 50.0, 50.0};
        auto rgb = QColor(Utils::ColorConversion::xyz_to_rgb(input, 100));

        REQUIRE(rgb.red() == Catch::Approx(204).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(183).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(180).epsilon(0.001));
    }

    SECTION("Convert XYZ (76.9984, 92.7825, 13.8526)"){
        QVector3D input = {76.9984, 92.7825, 13.8526};
        auto rgb = QColor(Utils::ColorConversion::xyz_to_rgb(input, 100));

        REQUIRE(rgb.red() == Catch::Approx(255).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(255).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(0).epsilon(0.001));
    }

    SECTION("Convert XYZ (53.8013, 78.7327, 106.9496)"){
        QVector3D input = {53.8013, 78.7327, 106.9496};
        auto rgb = QColor(Utils::ColorConversion::xyz_to_rgb(input, 100));

        REQUIRE(rgb.red() == Catch::Approx(0).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(255).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(255).epsilon(0.001));
    }

    SECTION("Convert XYZ (59.28935, 28.4848, 96.9638)"){
        QVector3D input = {59.28935, 28.4848, 96.9638};
        auto rgb = QColor(Utils::ColorConversion::xyz_to_rgb(input, 100));

        REQUIRE(rgb.red() == Catch::Approx(255).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(0).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(255).epsilon(0.001));
    }

    SECTION("Convert XYZ (13.686, 26.655, 16.281)"){
        QVector3D input = {13.686, 26.655, 16.281};
        auto rgb = QColor(Utils::ColorConversion::xyz_to_rgb(input, 100));

        REQUIRE(rgb.red() == Catch::Approx(0).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(165).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(99).epsilon(0.001));
    }

    SECTION("Convert XYZ (56.365, 68.318, 86.631)"){
        QVector3D input = {56.365, 68.318, 86.631};
        auto rgb = QColor(Utils::ColorConversion::xyz_to_rgb(input, 100));

        REQUIRE(rgb.red() == Catch::Approx(159).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(227).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(232).epsilon(0.001));
    }

    SECTION("Convert XYZ (20.6228, 10.63365, 0.9667)"){
        QVector3D input = {20.6228, 10.63365, 0.9667};
        auto rgb = QColor(Utils::ColorConversion::xyz_to_rgb(input, 100));

        REQUIRE(rgb.red() == Catch::Approx(188).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(0).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(0).epsilon(0.001));
    }

    SECTION("Convert XYZ (17.8788, 35.7576, 5.9596)"){
        QVector3D input = {17.8788, 35.7576, 5.9596};
        auto rgb = QColor(Utils::ColorConversion::xyz_to_rgb(input, 100));

        REQUIRE(rgb.red() == Catch::Approx(0).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(188).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(0).epsilon(0.001));
    }

    SECTION("Convert XYZ (9.02185, 3.60875, 47.5152)"){
        QVector3D input = {9.02185, 3.60875, 47.5152};
        auto rgb = QColor(Utils::ColorConversion::xyz_to_rgb(input, 100));

        REQUIRE(rgb.red() == Catch::Approx(0).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(0).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(188).epsilon(0.001));
    }

    SECTION("Convert XYZ (38.5016, 46.39125, 6.9263)"){
        QVector3D input = {38.5016, 46.39125, 6.9263};
        auto rgb = QColor(Utils::ColorConversion::xyz_to_rgb(input, 100));

        REQUIRE(rgb.red() == Catch::Approx(188).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(188).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(0).epsilon(0.001));
    }

    SECTION("Convert XYZ (29.64465, 14.2424, 48.4819)"){
        QVector3D input = {29.64465, 14.2424, 48.4819};
        auto rgb = QColor(Utils::ColorConversion::xyz_to_rgb(input, 100));

        REQUIRE(rgb.red() == Catch::Approx(188).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(0).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(188).epsilon(0.001));
    }

    SECTION("Convert XYZ (41.22857666, 59.3013316, 64.3964637)"){
        QVector3D input = {41.22857666, 59.3013316, 64.3964637};
        auto rgb = QColor(Utils::ColorConversion::xyz_to_rgb(input, 100));

        REQUIRE(rgb.red() == Catch::Approx(90).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(223).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(201).epsilon(0.001));
    }

    SECTION("Convert XYZ (0.2, 0.2, 0.2)"){
        QVector3D input = {18, 7.2, 95.1};
        auto rgb = Utils::ColorConversion::xyz_to_rgb(input, 100);

        REQUIRE(qRed(rgb) == Catch::Approx(0).epsilon(0.001));
        REQUIRE(qGreen(rgb)  == Catch::Approx(0).epsilon(0.001));
        REQUIRE(qBlue(rgb) == Catch::Approx(255).epsilon(0.001));
    }
}

TEST_CASE("Testing OKLAB to XYZ conversion function in ColorConversion","[ColorConversion][OKLAB_TO_XYZ]"){

    SECTION("Convert OKLab (0.5, 0.1, -0.1)") {
        constexpr double L = 0.5f, a = 0.1f, b = -0.1f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.144).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.25).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.107231781f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.7, 0.05, 0.05)") {
        constexpr double L = 0.7f, a = 0.05f, b = 0.05f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.355).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.49).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.407).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.3, -0.1, 0.1)") {
        constexpr double L = 0.3f, a = -0.1f, b = 0.1f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.018634673f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.09).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.037449498f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.4, 0.0, 0.0)") {
        constexpr double L = 0.4f, a = 0.0f, b = 0.0f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.060830083f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.16).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.069685124f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.6, 0.2, -0.2)") {
        constexpr double L = 0.6f, a = 0.2f, b = -0.2f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.284814477f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.36).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.144099638f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.8, -0.1, 0.1)") {
        constexpr double L = 0.8f, a = -0.1f, b = 0.1f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.405028522f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.64).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.65097338f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.9, 0.0, 0.0)") {
        constexpr double L = 0.9f, a = 0.0f, b = 0.0f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.692892551f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.81).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.793756962f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.2, 0.05, -0.05)") {
        constexpr double L = 0.2f, a = 0.05f, b = -0.05f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.008878949f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.04).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.007249821f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.1, -0.05, 0.05)") {
        constexpr double L = 0.1f, a = -0.05f, b = 0.05f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.000725046f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.01).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.001347069f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.95, 0.15, -0.15)") {
        constexpr double L = 0.95f, a = 0.15f, b = -0.15f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(1.003026962f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.9025).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.718033433f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.55, 0.0, 0.0)") {
        constexpr double L = 0.55f, a = 0.0f, b = 0.0f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.158134446f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.3025).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.181154102f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.75, 0.1, -0.1)") {
        constexpr double L = 0.75f, a = 0.1f, b = -0.1f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.470431209f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.5625).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.379788399f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.25, -0.05, 0.05)") {
        constexpr double L = 0.25f, a = -0.05f, b = 0.05f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.012623429f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.0625).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.019564914f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.85, 0.0, 0.0)") {
        constexpr double L = 0.85f, a = 0.0f, b = 0.0f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.583707392f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.7225).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.668677747f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.65, 0.15, -0.15)") {
        constexpr double L = 0.65f, a = 0.15f, b = -0.15f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.333868533f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.4225).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.215569973f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.35, -0.1, 0.1)") {
        constexpr double L = 0.35f, a = -0.1f, b = 0.1f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.030419031f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.1225).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.058520034f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.15, 0.0, 0.0)") {
        constexpr double L = 0.15f, a = 0.0f, b = 0.0f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.003207837f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.0225).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.003674802f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.45, 0.05, -0.05)") {
        constexpr double L = 0.45f, a = 0.05f, b = -0.05f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.096295029f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.2025).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.088126533f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.8, 0.1, -0.1)") {
        constexpr double L = 0.8f, a = 0.1f, b = -0.1f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.568252742f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.64).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.463988572f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.2, -0.1, 0.1)") {
        constexpr double L = 0.2f, a = -0.1f, b = 0.1f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.005053382f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.04).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.011632279f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.9, -0.1, 0.1)") {
        constexpr double L = 0.9f, a = -0.1f, b = 0.1f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.583336592f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.81).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx( 0.919261038f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.6, -0.1, 0.1)") {
        constexpr double L = 0.6f, a = -0.1f, b = 0.1f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.165545061f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.36).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.280731112f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.3, 0.1, -0.1)") {
        constexpr double L = 0.3f, a = 0.1f, b = -0.1f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.032690708f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.09).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.021347325f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.7, -0.1, 0.1)") {
        constexpr double L = 0.7f, a = -0.1f, b = 0.1f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.267562479f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.49).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.440425724f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.4, -0.1, 0.1)") {
        constexpr double L = 0.4f, a = -0.1f, b = 0.1f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.046402965f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.16).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.086212404f).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.5, -0.1, 0.1)") {
        constexpr double L = 0.5f, a = -0.1f, b = 0.1f;
        auto xyz = Utils::ColorConversion::oklab_to_xyz(L, a, b);

        REQUIRE(xyz.x() == Catch::Approx(0.093605608f).epsilon(0.001));
        REQUIRE(xyz.y() == Catch::Approx(0.25f).epsilon(0.001));
        REQUIRE(xyz.z() == Catch::Approx(0.164975703f).epsilon(0.001));
    }

}

TEST_CASE("Testing OKLAB to RGB conversion function in ColorConversion","[ColorConversion][OKLAB_TO_RGB]"){
    SECTION("Convert OKLab (0.5, -0.1, 0.1)") {
        constexpr double L = 0.5f, a = -0.1f, b = 0.1f;
        QColor rgb = Utils::ColorConversion::oklab_to_rgb(L, a, b);

        REQUIRE(rgb.red() == Catch::Approx(0).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(167).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(100).epsilon(0.001));
    }


    SECTION("Convert OKLab (0.4, -0.4, 0.4)") {
        constexpr double L = 0.4f, a = -0.4f, b = 0.4f;
        QColor rgb = Utils::ColorConversion::oklab_to_rgb(L, a, b);

        REQUIRE(rgb.red() == Catch::Approx(0).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(150).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(94).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.3, -0.2, 0.2)") {
        constexpr double L = 0.3f, a = -0.2f, b = 0.2f;
        QColor rgb = Utils::ColorConversion::oklab_to_rgb(L, a, b);

        REQUIRE(rgb.red() == Catch::Approx(0).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(111).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(49).epsilon(0.001));
    }

    SECTION("Convert OKLab (0.1, -0.1, 0.1)") {
        constexpr double L = 0.1f, a = -0.1f, b = 0.1f;
        QColor rgb = Utils::ColorConversion::oklab_to_rgb(L, a, b);

        REQUIRE(rgb.red() == Catch::Approx(0).epsilon(0.001));
        REQUIRE(rgb.green() == Catch::Approx(37).epsilon(0.001));
        REQUIRE(rgb.blue() == Catch::Approx(0).epsilon(0.001));
    }
}
