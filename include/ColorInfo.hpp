#ifndef COLORINFO_HPP
#define COLORINFO_HPP
#include <QWidget>
#include <vector>

class ColorInfo : public QWidget{
    Q_OBJECT
private:
    std::shared_ptr<QColor> m_ColorContext;
    QString m_RgbaFormat;
    std::vector<QMetaObject::Connection> m_Connections;
public:
    explicit ColorInfo(std::shared_ptr<QColor> color, QWidget* parent = Q_NULLPTR);
    ~ColorInfo();

    void SetColorContext(std::shared_ptr<QColor> color);
    std::shared_ptr<QColor> GetColorContext() const;

public slots:
    void SetupColorInfo();
signals:
    void InfoChanged();
private:
    void SetupConnections();
    void DisconnectAll();
};

#endif // COLORINFO_HPP
