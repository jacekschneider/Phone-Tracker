#include "include/ledwidget.h"
#include <QPainter>

LedWidget::LedWidget(QWidget *parent) : QWidget(parent), isOn(false)
{
    setFixedSize(20, 20);
}

void LedWidget::setOn(bool on)
{
    if (isOn != on) {
        isOn = on;
        update(); // Trigger a repaint
    }
}

void LedWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QColor ledColor = isOn ? Qt::green : Qt::red;
    painter.setPen(Qt::NoPen);
    painter.setBrush(ledColor);
    painter.drawEllipse(rect());
}
