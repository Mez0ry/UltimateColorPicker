#include "GridLayout.hpp"
#include <queue>
#include <variant>

GridLayout::GridLayout(int rows, int cols, QWidget *parent) : QGridLayout(parent), m_GridSize(cols,rows){

}

void GridLayout::PushWidget(QWidget *w, Qt::Alignment alignment){
    auto p = GetNextInsertionIndex();
    this->addWidget(w, p.y(),p.x(),alignment);
}

void GridLayout::PushWidget(QWidget *w, int row_span, int column_span, Qt::Alignment alignment){

    auto p = GetNextInsertionIndex();
    this->addWidget(w, p.y(),p.x(),row_span,column_span,alignment);
}

void GridLayout::PushItem(QLayoutItem *item, int row_span, int column_span, Qt::Alignment alignment){
    auto p = GetNextInsertionIndex();

    this->addItem(item,p.y(),p.x(),row_span,column_span,alignment);
}

QSize GridLayout::GetGridSize() const
{
    return m_GridSize;
}

void GridLayout::SetGridSize(QSize size)
{
    m_GridSize = size;
    emit GridSizeChanged(m_GridSize);
}

void GridLayout::Clear()
{
    while(this->count()) {
        QWidget* widget = this->itemAt(0)->widget();
        QLayoutItem* item = this->itemAt(0);
        if( widget ) {
            this->removeWidget(widget);
            delete widget;
        }else if(item){
            this->removeItem(item);
            delete item;
        }
    }
}

bool GridLayout::IsEmpty() const
{
    return (GetNextInsertionIndex().x() == 0 && GetNextInsertionIndex().y() == 0);
}

void GridLayout::UpdateWidgetsToGridSize(QSize new_size)
{
    Q_UNUSED(new_size);

    std::queue<std::variant<QWidget*,QLayoutItem*>> item_widget_var;

    auto max_size = this->count();

    while(this->count()) {
        QWidget* widget = this->itemAt(0)->widget();
        QLayoutItem* item = this->itemAt(0);

        if( widget ) {
            this->removeWidget(widget);
            item_widget_var.push (std::variant<QWidget*,QLayoutItem*>(widget));
        }else if(item){
            this->removeItem(item);
            item_widget_var.push(std::variant<QWidget*,QLayoutItem*>(item));
        }
    }

    auto count = 0;
    while( count < max_size && item_widget_var.size() > size_t(0)){
        QPoint insert_pos(-1,-1);
        insert_pos.setX(count % m_GridSize.width());
        insert_pos.setY(count / m_GridSize.width());

        auto variant = item_widget_var.front();
        if(auto widget_ptr = std::get_if<QWidget*>(&variant)){
            this->addWidget(*widget_ptr,insert_pos.y(),insert_pos.x(),1,1, Qt::AlignCenter);
        }else if(auto item_ptr = std::get_if<QLayoutItem*>(&variant)){
            this->addItem(*item_ptr,insert_pos.y(),insert_pos.x(),1,1,(*item_ptr)->alignment());
        }

        item_widget_var.pop();
        ++count;
    }
}

const QPoint GridLayout::GetNextInsertionIndex() const {
    if(this->count() == 0){
        return QPoint(0,0);
    }
    QPoint insert_pos(-1,-1);
    insert_pos.setX(this->count() % m_GridSize.width());
    insert_pos.setY(this->count() / m_GridSize.width());
    return insert_pos;
}
