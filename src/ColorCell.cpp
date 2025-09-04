#include "ColorCell.hpp"
#include <QGuiApplication>
#include <QClipboard>

ColorCell::ColorCell(QColor color, QSize size, QWidget *parent) : QLabel(parent), m_Size(size)
{
    this->SetColor(color);
}

void ColorCell::SetColor(QColor color)
{
    QPixmap pixmap(m_Size);
    pixmap.fill(color);
    this->setPixmap(pixmap);
}

void ColorCell::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    auto color = this->pixmap().toImage().pixelColor(0,0);
    QGuiApplication::clipboard()->setText(color.name(QColor::HexRgb).toUpper());
}
