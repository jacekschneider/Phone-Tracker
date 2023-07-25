#ifndef LEDWIDGET_H
#define LEDWIDGET_H

#include <QWidget>

class LedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LedWidget(QWidget *parent = nullptr);

    void setOn(bool on);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    bool isOn;
};

#endif // LEDWIDGET_H
