#include "spatial_pointer.h"
#include "ui_spatial_pointer.h"
#include "phidget_spatial.h"
#include <QTimer>
#include <QCursor>
#include <QDesktopServices>
#include <Qurl>

SpatialPointer::SpatialPointer(QWidget *parent) : QWidget(parent), ui(new Ui::SpatialPointer)
{
    ui->setupUi(this);

    spatial_ = PhidgetSpatial::instance();

    tmr_update = new QTimer(this);
    connect(tmr_update, SIGNAL(timeout()), this, SLOT(slot_update()));

    tolerance_ = ui->sld_deadzone->value();
    ui->lbl_deadzone_value->setText(QString::number(ui->sld_deadzone->value()));

    speed_ = ui->sld_speed->value();
    ui->lbl_speed_value->setText(QString::number(ui->sld_speed->value()));

    enabled_ = false;
    set_status(kStatusIdle);
}

SpatialPointer::~SpatialPointer()
{
    delete tmr_update;
    delete ui;
}

void SpatialPointer::slot_update()
{
    if(!spatial_->attatched())
    {
        set_enabled(false);
        set_status(kStatusFail);
    }

    if(!enabled_)
    {
        return;
    }

    auto angular_rate = spatial_->angular_rate();

    int velocity_x = (angular_rate.z > tolerance_ || angular_rate.z < -tolerance_) ? angular_rate.z * speed_ : 0;
    int velocity_y = (angular_rate.x > tolerance_ || angular_rate.x < -tolerance_) ? angular_rate.x * speed_ : 0;

    move_cursor(velocity_x, velocity_y);
}

void SpatialPointer::on_sld_deadzone_valueChanged(int value)
{
    tolerance_= value;
    ui->lbl_deadzone_value->setText(QString::number(value));
}

void SpatialPointer::on_sld_speed_valueChanged(int value)
{
    speed_ = value;
    ui->lbl_speed_value->setText(QString::number(value));
}


void SpatialPointer::on_btn_enable_clicked()
{
    if(!spatial_->attatched() && !spatial_->initialize(8, 1))
    {
        set_status(kStatusFail);
        return;
    }

    set_enabled(true);
}

void SpatialPointer::on_btn_disable_clicked()
{
    set_enabled(false);
}

void SpatialPointer::set_status(const QString &status)
{
    ui->lbl_state->setText(status);
}

void SpatialPointer::set_enabled(const bool &state)
{
    enabled_ = state;
    enabled_ ? tmr_update->start(kUpdateRate) : tmr_update->stop();

    ui->btn_enable->setEnabled(state == false);
    ui->btn_disable->setEnabled(state == true);

    set_status(state ? kStatusWorking : kStatusIdle);
}

void SpatialPointer::move_cursor(const int &x, const int &y)
{
    QPoint cursor_position = QCursor::pos();
    QCursor::setPos(cursor_position.x() + x, cursor_position.y() + y);
}

void SpatialPointer::on_btn_github_clicked()
{
    QDesktopServices::openUrl(QUrl(ui->btn_github->text()));
}
