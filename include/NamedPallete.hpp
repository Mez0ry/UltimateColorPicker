#ifndef NAMEDPALLETE_H
#define NAMEDPALLETE_H
#include <QLabel>
#include "DeletePopup.hpp"
#include <QJsonObject>

class NamedPallete : public QLabel{
    Q_OBJECT
private:
    QSize m_Size;
    int m_Radius{16};
    QString m_PalleteName;
    QJsonObject m_ColorValues;
    DeletePopup m_DeletePopup;
public:
    explicit NamedPallete(const QString& pallete_name,QJsonObject color_values = QJsonObject(),QWidget* parent = Q_NULLPTR);

    ~NamedPallete() Q_DECL_EQ_DEFAULT;

    void Serialize(QJsonDocument doc, const QString& file_path);
    static void Serialize(const NamedPallete* pallete, QJsonDocument& doc, const QString& file_path);

    QJsonObject GetColors() const;
    void SetColors(QJsonObject colors);

    const QString& GetPalleteName() const;
    void SetPalleteName(const QString& pallete_name);
signals:
    void DeleteClicked(NamedPallete* const pallete_to_remove);
    void Clicked();
protected:
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
private:
};
#endif // NAMEDPALLETE_H
