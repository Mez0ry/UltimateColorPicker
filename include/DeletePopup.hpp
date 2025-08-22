#ifndef DELETEPOPUP_H
#define DELETEPOPUP_H
#include <QLabel>

class DeletePopup : public QLabel{
    Q_OBJECT
private:
public:
    DeletePopup(QWidget* parent = nullptr);

    ~DeletePopup();

    void OnRightClick(int border_length = 19);
signals:
    void LeftButtonClicked();

protected:
    void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;

    void keyReleaseEvent(QKeyEvent*) Q_DECL_OVERRIDE;
private:
};

#endif // DELETEPOPUP_H
