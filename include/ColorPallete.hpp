#ifndef COLORPALLETE_HPP
#define COLORPALLETE_HPP
#include <QWidget>
#include <mutex>

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
    QFrame* m_BackgroundVBoxLayout{nullptr};
    QFrame* m_Content{nullptr};
    std::mutex m_Mutex;

    struct SlidingWindow{
        SlidingWindow(int start, int end, int window_size) : start_idx(start), end_idx(end), elems_window_size(window_size){}
        int start_idx,end_idx;
        const int elems_window_size;
    };

    SlidingWindow m_PalleteSlidingWindow;
    int m_CurrentPage{0};
    int m_MaximumPages{0};
    QScrollArea* m_ScrollArea{nullptr};
    AddIcon* m_AddIcon{nullptr};
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
