#ifndef COLORCOMBO_HPP
#define COLORCOMBO_HPP
#include <QWidget>
#include <QColor>
#include <vector>
#include <unordered_map>

class QComboBox;
class QGridLayout;

enum class ColorType : quint8{
    COMPLEMENTARY = 0,
    SPLIT_COMPLEMENTARY,
    TRIADIC,
    TETRADIC,
    ANALOGOUS
};

class ColorCombo : public QWidget{
    Q_OBJECT
private:
    QColor* m_BaseColor;
    QComboBox* m_ComboBox;
    QGridLayout* m_ColorComboGrid;
    int m_CurrentComboIndex{0};
    std::unordered_map<int,std::vector<QColor>> m_ColorTypesMap;

public:
    explicit ColorCombo(QWidget* parent = Q_NULLPTR);
    ~ColorCombo() Q_DECL_EQ_DEFAULT;

    void SetBaseColor(QColor* base_color);
signals:
    void BaseColorChanged();
public slots:
    void UpdateColor();
private:
    void ClearGridLayout(QGridLayout* layout);
    void AddColorCellsToLayout(QGridLayout* layout, std::vector<QColor>& color);
};

#endif // COLORCOMBO_HPP
