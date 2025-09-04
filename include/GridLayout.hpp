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
    GridLayout(int rows, int cols,QWidget* parent = Q_NULLPTR);

    ~GridLayout() Q_DECL_EQ_DEFAULT;

    void PushWidget(QWidget* w,Qt::Alignment alignment = Qt::Alignment() );
    void PushWidget(QWidget *w, int row_span = 0, int column_span = 0, Qt::Alignment alignment = Qt::Alignment());
    void PushItem(QLayoutItem* item,int row_span = 0, int column_span = 0, Qt::Alignment alignment = Qt::Alignment());

    QSize GetGridSize() const;
    void SetGridSize(QSize size);

    const QPoint GetNextInsertionIndex() const;

    void Clear();

    bool IsEmpty() const;

    quint32 GetMaxWidth() const{
        if(IsEmpty()){
            return 0;
        }

        quint32 res = 0;
        int count = 0;
        auto spacing = this->spacing();

        while(count < this->count()){
            auto* item = this->itemAt(count);

            if( item ) {
                QWidget* widget = item->widget();
                if(widget)
                    res += (widget->width() + spacing);
            }

            ++count;
        }

        return res;
    }

    quint32 GetRowTotalWidth() const{
        if(isEmpty()){
            return 0;
        }

        auto next_insert_index = GetNextInsertionIndex();
        auto width_size = (next_insert_index.x() > 0 && next_insert_index.y() == 0) ? next_insert_index.x() - 1 : m_GridSize.width();
        auto spacing = this->spacing();

        if(width_size >= this->count()){
            width_size = this->count() - 1;
        }

        int i = width_size;
        quint32 res = 0;

        for(; i >= 0;--i){
            auto* item = this->itemAt(i);

            if( item ) {
                QWidget* widget = item->widget();
                res += widget->width() + spacing;
            }
        }

        return res;

    }

public slots:
    void UpdateWidgetsToGridSize(QSize new_size);
signals:
    void GridSizeChanged(QSize new_size);
private:
};

#endif // GRIDLAYOUT_H
