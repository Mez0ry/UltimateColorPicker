#ifndef ADDICON_HPP
#define ADDICON_HPP
#include <QLineEdit>
#include <QLabel>

class PalleteNameSpecifierPopup : public QLineEdit{
    Q_OBJECT
private:

public:
    PalleteNameSpecifierPopup(QWidget* parent = Q_NULLPTR);

    ~PalleteNameSpecifierPopup() Q_DECL_EQ_DEFAULT;
signals:
    void PressedOutside();
protected:
    void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
private:
};

class AddIcon : public QLabel{
    Q_OBJECT
private:
    QSize m_Size;
    PalleteNameSpecifierPopup m_PalleteNamePopup;
    QString m_TextToAdd;
public:
    explicit AddIcon(QWidget* parent = Q_NULLPTR,QSize size = {32,32});

    ~AddIcon() Q_DECL_EQ_DEFAULT;

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
#endif // ADDICON_HPP
