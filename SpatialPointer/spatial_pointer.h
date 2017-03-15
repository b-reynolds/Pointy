#ifndef SPATIA_LPOINTER_H
#define SPATIA_LPOINTER_H

#include <QWidget>
#include <phidget21.h>

namespace Ui {
    class SpatialPointer;
}

class PhidgetSpatial;

class SpatialPointer : public QWidget
{
    Q_OBJECT

public:

    explicit SpatialPointer(QWidget *parent = 0);
    ~SpatialPointer();

private slots:

    void slot_update();

    void on_sld_deadzone_valueChanged(int value);
    void on_sld_speed_valueChanged(int value);

    void on_btn_enable_clicked();
    void on_btn_disable_clicked();
    void on_btn_github_clicked();

private:

    Ui::SpatialPointer *ui;

    QTimer* tmr_update;

    PhidgetSpatial* spatial_;

    const int kCheckStatusRate = 250;
    const int kUpdateRate = 10;

    const QString kStatusIdle = "Disabled";
    const QString kStatusFail = "Please ensure your spatial sensor is attatched";
    const QString kStatusWorking = "Enabled";

    int tolerance_;
    int speed_;
    bool enabled_;

    void set_status(const QString& status);
    void set_enabled(const bool& state);

    void move_cursor(const int& x, const int& y);

};

#endif // SPATIA_LPOINTER_H
