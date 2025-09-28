#ifndef COLORINFO_HPP
#define COLORINFO_HPP
#include <QWidget>
#include <vector>
#include "SingletonBase.hpp"

class ColorInfo : public QWidget , public SingletonBase<ColorInfo>{
    Q_OBJECT
private:
    std::shared_ptr<QColor> m_ColorContext;
    QString m_RgbaFormat{"rgba(%r, %g, %b, %a)"};
    std::vector<QMetaObject::Connection> m_Connections;
public:
    ColorInfo() = default;
    explicit ColorInfo(std::shared_ptr<QColor> color, QWidget* parent = Q_NULLPTR);
    ~ColorInfo();

    void Setup();
    void SetColorContext(std::shared_ptr<QColor> color);
public slots:
    void SetupColorInfo();
signals:
    void InfoChanged();
private:
    void SetupConnections();
    void DisconnectAll();
};

#endif // COLORINFO_HPP
