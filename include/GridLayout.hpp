#ifndef GRIDLAYOUT_H
#define GRIDLAYOUT_H
#include <QGridLayout>
#include <QWidget>

class QLayoutItem;
class QWidget;

class GridLayout : public QGridLayout{
    Q_OBJECT
private:
    QSize m_GridSize;
public:
    GridLayout(int rows, int cols, QWidget* parent = Q_NULLPTR);

    ~GridLayout() Q_DECL_EQ_DEFAULT;

    void PushWidget(QWidget* w,Qt::Alignment alignment = Qt::Alignment() );
    void PushWidget(QWidget *w, int row_span = 0, int column_span = 0, Qt::Alignment alignment = Qt::Alignment());
    void PushItem(QLayoutItem* item,int row_span = 0, int column_span = 0, Qt::Alignment alignment = Qt::Alignment());

    QSize GetGridSize() const;
    void SetGridSize(QSize size);

    const QPoint GetNextInsertionPos() const;

    void Clear();
    bool IsEmpty() const;

    quint32 GetMaxWidth() const;
    quint32 GetRowTotalWidth() const;
public slots:
    void UpdateWidgetsToGridSize(QSize new_size);
signals:
    void GridSizeChanged(QSize new_size);
private:
};

#endif // GRIDLAYOUT_H
