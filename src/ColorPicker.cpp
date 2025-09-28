#include "ColorPicker.hpp"
#include <QApplication>
#include <QClipboard>
#include <QCursor>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QScreen>
#include <QTimer>
#include <QCloseEvent>

ColorPicker::ColorPicker(QWidget *parent)
    : QWidget(parent)
{

}

ColorPicker::~ColorPicker(){
    releaseMouse();
    releaseKeyboard();
    close();
}

void ColorPicker::Start()
{
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_AlwaysStackOnTop);
    setMouseTracking(true);
    setCursor(Qt::CrossCursor);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setWindowModality(Qt::ApplicationModal);
    setFocusPolicy(Qt::StrongFocus);
    setGeometry(CurrentScreenLogicalGeometry());
    showFullScreen();
    activateWindow();
    grabMouse();
    grabKeyboard();

    m_Timer = new QTimer(this);
    m_TimerConnection = connect(m_Timer, &QTimer::timeout, this, [this]{ SampleUnderCursor(); update(); });
    m_Timer->start(16);
}

void ColorPicker::OnButtonPush()
{
    this->show();
    this->Start();
}

QRect ColorPicker::CurrentScreenLogicalGeometry() const {
    const QScreen* s = QGuiApplication::screenAt(QCursor::pos());
    if (!s) s = QGuiApplication::primaryScreen();
    return s ? s->geometry() : QRect(0, 0, 0, 0);
}

void ColorPicker::SampleUnderCursor() {
    m_CursorGlobal = QCursor::pos();
    QScreen* const s = QGuiApplication::screenAt(m_CursorGlobal);

    if (!s)
        return;

#if defined(Q_OS_WIN)
    const QRect vgeom = QGuiApplication::primaryScreen()->virtualGeometry();
    const int gx = std::clamp(m_CursorGlobal.x(), vgeom.left(),  vgeom.right());
    const int gy = std::clamp(m_CursorGlobal.y(), vgeom.top(),   vgeom.bottom());
    const QPixmap px1 = s->grabWindow(0, gx, gy, 1, 1);

    if (px1.isNull())
        return;

    const QImage i1 = px1.toImage();
    if (i1.isNull())
        return;

    m_SelectedColor = i1.pixelColor(0, 0);

    // const int size = m_Grid * 2 + 1;
    // QPixmap around = s->grabWindow(0, gx - m_Grid, gy - m_Grid, size, size);

    // if (!around.isNull())
    //     m_MagnifierSrc = around;
#else
    const QRect geom = s->geometry();
    const qreal dpr  = s->devicePixelRatio();
    int nx = int((m_CursorGlobal.x() - geom.x()) * dpr);
    int ny = int((m_CursorGlobal.y() - geom.y()) * dpr);
    const QSize ns = (geom.size() * dpr).toSize();
    nx = std::clamp(nx, 0, ns.width()  - 1);
    ny = std::clamp(ny, 0, ns.height() - 1);
    QPixmap px1 = s->grabWindow(0, nx, ny, 1, 1);
    if (px1.isNull()) return;
    QImage i1 = px1.toImage();
    if (i1.isNull()) return;
    m_SelectedColor = i1.pixelColor(0, 0);

    // const int size = m_Grid * 2 + 1;
    // QPixmap around = s->grabWindow(0, nx - m_Grid, ny - m_Grid, size, size);
    // if (!around.isNull()) m_MagnifierSrc = around;
#endif
}

void ColorPicker::paintEvent(QPaintEvent*) {
    setGeometry(CurrentScreenLogicalGeometry());
    showFullScreen();
    activateWindow();
    grabMouse();
    grabKeyboard();

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const QPoint center = mapFromGlobal(m_CursorGlobal);
    const int dia = m_Radius * 2;

    QPainterPath ring;
    ring.setFillRule(Qt::OddEvenFill);
    ring.addEllipse(QRect(center.x() - m_Radius, center.y() - m_Radius, dia, dia));

    ring.addEllipse(QRect(center.x() - m_Hole,   center.y() - m_Hole,   m_Hole * 2, m_Hole * 2));

    p.setClipPath(ring);
    p.fillPath(ring, QBrush(m_SelectedColor));
    p.setClipping(false);

    p.setBrush(Qt::NoBrush);
    p.setPen(QPen(Qt::white, 1));
    p.drawLine(center.x() - m_Radius, center.y(), center.x() - m_Hole - 1, center.y());
    p.drawLine(center.x() + m_Hole + 1, center.y(), center.x() + m_Radius, center.y());
    p.drawLine(center.x(), center.y() - m_Radius, center.x(), center.y() - m_Hole - 1);
    p.drawLine(center.x(), center.y() + m_Hole + 1, center.x(), center.y() + m_Radius);

    const QString hex = m_SelectedColor.name(QColor::HexRgb).toUpper();
    const QString rgb = QString("RGB(%1, %2, %3, %4)")
                            .arg(m_SelectedColor.red())
                            .arg(m_SelectedColor.green())
                            .arg(m_SelectedColor.blue()).arg(m_SelectedColor.alpha());

    const QString hud = hex + "  •  " + rgb + "   (Click to pick, Esc to cancel)";

    const QRect tr(20, 20, width() - 40, 40);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0,0,0,160));
    p.drawRoundedRect(tr.adjusted(-6,-6,6,6), 8, 8);
    p.setPen(Qt::white);
    p.drawText(tr, Qt::AlignLeft | Qt::AlignVCenter, hud);
}

void ColorPicker::mouseMoveEvent(QMouseEvent* e) {
    m_CursorGlobal = e->globalPosition().toPoint();
    update();
}

void ColorPicker::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        QGuiApplication::clipboard()->setText(m_SelectedColor.name(QColor::HexRgb).toUpper());

        emit ColorPicked(m_SelectedColor);

        e->accept();
        releaseMouse();
        releaseKeyboard();
        close();
    } else if (e->button() == Qt::RightButton) {
        e->accept();
        releaseMouse();
        releaseKeyboard();
        close();
    } else {
        QWidget::mousePressEvent(e);
    }
}

void ColorPicker::mouseReleaseEvent(QMouseEvent* e) {
    e->accept();
}

void ColorPicker::keyReleaseEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Escape) {
        releaseMouse();
        releaseKeyboard();
        close();
    }
}

bool ColorPicker::event(QEvent* ev) {
    if (ev->type() == QEvent::ScreenChangeInternal) {
        setGeometry(CurrentScreenLogicalGeometry());
    }
    return QWidget::event(ev);
}

void ColorPicker::closeEvent(QCloseEvent* e) {
    disconnect(m_TimerConnection);
    releaseMouse();
    releaseKeyboard();
    QWidget::closeEvent(e);
}
