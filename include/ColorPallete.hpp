#ifndef COLORPALLETE_HPP
#define COLORPALLETE_HPP
#include <QWidget>

class QFrame;
class QVBoxLayout;

class DeletePopup;
class NamedPallete;
class ColoredEllipse;
class PalleteNameSpecifierPopup;
class AddIcon;

class QScrollArea;

class ColorPallete : public QWidget{
    Q_OBJECT
private:
    QSize m_Size;
    QFrame* m_BackgroundVBoxLayout{nullptr};
    QScrollArea* m_ScrollArea{nullptr};
    AddIcon* m_AddIcon{nullptr};

    QFrame* m_Content{nullptr};
public:
    explicit ColorPallete(QWidget* parent = Q_NULLPTR);
    ~ColorPallete() Q_DECL_EQ_DEFAULT;

    void OnButtonClicked();
protected:
    void moveEvent(QMoveEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
private:
    void OnResize(QSize new_size);
    void UpdateColorPalletes();
};

#endif // COLORPALLETE_HPP
