#ifndef COLORPALLETE_HPP
#define COLORPALLETE_HPP
#include <QWidget>
#include "SlidingWindow.hpp"

template<typename TypeToWatch>
class QFutureWatcher;

template<typename TypeToWatch>
class QFuture;

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
    QFrame* m_BackgroundVBoxLayout{Q_NULLPTR};
    QFrame* m_Content{Q_NULLPTR};

    SlidingWindow m_PalleteSlidingWindow;
    int m_CurrentPage{0};
    int m_MaximumPages{0};
    QScrollArea* m_ScrollArea{Q_NULLPTR};
    AddIcon* m_AddIcon{Q_NULLPTR};
    QFutureWatcher<QJsonDocument>* m_JsonWatcher;
    std::shared_ptr<QJsonDocument> m_JsonDocument;
    QStringList m_AllJsonKeys;
public:
    explicit ColorPallete(QWidget* parent = Q_NULLPTR);
    ~ColorPallete() Q_DECL_EQ_DEFAULT;

    void OnButtonClicked();
private:
    void SetupBackgroundVBoxLayout();
    void SetupContent();
    void SetupScrollArea();
    void SetupAddIcon();
private:
    void OnNamedPalleteClicked(NamedPallete* const pallete_ptr);
protected:
    void moveEvent(QMoveEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
private:
    void OnResize(QSize new_size);
    void AddNamedPalleteToLayout(const QString& key, const QJsonObject& color_values);
};

#endif // COLORPALLETE_HPP
