#ifndef OVERLAY_H
#define OVERLAY_H

#include <QWidget>

namespace Ui {
class Overlay;
}

class Overlay : public QWidget
{
    Q_OBJECT

public:

    explicit Overlay(QWidget *parent = 0);
    ~Overlay();

    void set_enabled(const bool& state);

private slots:

    void slot_update();

private:

    const float kOneSecond = 1000.0f;
    const int kCountdownTime = 3;

    Ui::Overlay *ui;

    QTimer* tmr_update;

    void set_countdown(const int& value);

    int countdown_;

};

#endif // OVERLAY_H
