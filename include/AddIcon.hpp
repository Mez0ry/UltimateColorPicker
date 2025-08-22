#ifndef ADDICON_H
#define ADDICON_H
#include <QLineEdit>
#include <QLabel>

class PalleteNameSpecifierPopup : public QLineEdit{
    Q_OBJECT
private:

public:
    PalleteNameSpecifierPopup(QWidget* parent = nullptr);

    ~PalleteNameSpecifierPopup() = default;
signals:
    void PressedOutside();
protected:
    void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
private:
};

class AddIcon : public QLabel{
    Q_OBJECT
private:
    int m_Height,m_Width;
    PalleteNameSpecifierPopup m_PalleteNamePopup;
    QString m_TextToAdd;
public:
    explicit AddIcon(QWidget* parent = nullptr);

    ~AddIcon() = default;

    void OnParentGeometryUpdate(QRect rect);
    void OnParentSizeUpdate(QSize size);

    void SetPlaceHolderText(QString placeholder);

signals:
    void WasNotNamed();
    void IsNamed(QString new_name);
    void IsLeftClicked();
protected:
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
private:
};
#endif // ADDICON_H
