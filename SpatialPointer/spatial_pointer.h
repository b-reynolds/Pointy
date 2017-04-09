#ifndef SPATIA_LPOINTER_H
#define SPATIA_LPOINTER_H

#include <QWidget>
#include <QMessageBox>
#include <phidget21.h>
#include "overlay.h"

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
    void slot_activate_click();

    void on_sld_deadzone_valueChanged(int value);
    void on_sld_speed_valueChanged(int value);

    void on_btn_enable_clicked();
    void on_btn_disable_clicked();

    void on_chk_vertical_stateChanged(int arg1);

    void on_chk_horizontal_stateChanged(int arg1);

    void on_chk_invert_toggled(bool checked);

    void on_sld_trigger_radius_valueChanged(int value);

    void on_sld_trigger_time_valueChanged(int value);

    void on_sld_click_time_valueChanged(int value);

    void on_chk_clicking_enabled_toggled(bool checked);

private:



    Ui::SpatialPointer *ui;

    QTimer* tmr_update;
    QTimer* tmr_activate_click;

    PhidgetSpatial* spatial_;

    Overlay* overlay_;

    const int kUpdateRate = 10;

    const int kStartClickRadius = 100;
    const float kActivateClickTime = 1000.0f;

    const QString kStatusIdle = "Click the green arrow to begin pointing";
    const QString kStatusFail = "Please ensure your spatial sensor is attatched";
    const QString kStatusWorking = "Active";

    int tolerance_;
    int speed_;
    int radius_;
    int trigger_time_;
    int click_time_;

    bool horizontal_;
    bool vertical_;
    bool invert_;
    bool enabled_;
    bool clicking_enabled_;

    void set_enabled(const bool& state);

    void move_cursor(const int& x, const int& y);

    int abs_difference(const int& x, const int& y);

    void show_message_box(const QString& message, const QString& caption, const QMessageBox::Icon& icon);

    QPoint old_mouse_position;

};

#endif // SPATIA_LPOINTER_H
