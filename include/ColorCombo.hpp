#ifndef COLORCOMBO_H
#define COLORCOMBO_H
#include <QWidget>
#include <QComboBox>
#include <QGridLayout>
#include <QColor>

#include <vector>
#include <unordered_map>

enum class ColorType{
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
    explicit ColorCombo(QWidget* parent = nullptr);
    ~ColorCombo();

    void SetBaseColor(QColor* base_color);
signals:
    void BaseColorChanged();
public slots:
    void UpdateColor();
private:
    void ClearGridLayout(QGridLayout* layout);
    void AddColorCellsToLayout(QGridLayout* layout, std::vector<QColor>& color);
};

#endif // COLORCOMBO_H
