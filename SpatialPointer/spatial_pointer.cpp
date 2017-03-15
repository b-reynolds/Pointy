#include "spatial_pointer.h"
#include "ui_spatial_pointer.h"
#include "phidget_spatial.h"
#include <QTimer>
#include <QCursor>
#include <QDesktopServices>
#include <Qurl>

/**
 * @brief Initialize
 * @param parent parent widget
 */
SpatialPointer::SpatialPointer(QWidget *parent) : QWidget(parent), ui(new Ui::SpatialPointer)
{
    ui->setupUi(this);

    // Obtain a reference to the PhidgetSpatial singleton
    spatial_ = PhidgetSpatial::instance();

    // Initialize and connect the update timer to the update function
    tmr_update = new QTimer(this);
    connect(tmr_update, SIGNAL(timeout()), this, SLOT(slot_update()));

    // Initialize the tolerance value and respective controls to their default values
    tolerance_ = ui->sld_deadzone->value();
    ui->lbl_deadzone_value->setText(QString::number(ui->sld_deadzone->value()));

    // Initialize the speed value and respective controls to their default values
    speed_ = ui->sld_speed->value();
    ui->lbl_speed_value->setText(QString::number(ui->sld_speed->value()));

    // Set the status to idle
    enabled_ = false;
    set_status(kStatusIdle);
}

/**
 * @brief Clean up
 */
SpatialPointer::~SpatialPointer()
{
    delete tmr_update;
    delete ui;
}

/**
 * @brief Moves the mouse cursor using the angular data of the Phidget Spatial
 */
void SpatialPointer::slot_update()
{
    // Phidget Spatial was detatched, disable and set the status message to failure
    if(!spatial_->attatched())
    {
        set_enabled(false);
        set_status(kStatusFail);
    }

    // Pointer was disabled, return
    if(!enabled_)
    {
        return;
    }

    // Obtain the angular rate data from the Phidget Spatial
    auto angular_rate = spatial_->angular_rate();

    // If the angular data on either axis exceeds the tolerence then set their respective velocitys
    int velocity_x = (angular_rate.z > tolerance_ || angular_rate.z < -tolerance_) ? angular_rate.z * speed_ : 0;
    int velocity_y = (angular_rate.x > tolerance_ || angular_rate.x < -tolerance_) ? angular_rate.x * speed_ : 0;

    // Move the cursor
    move_cursor(velocity_x, velocity_y);
}

/**
 * @brief Set the status bar text
 * @param status new status
 */
void SpatialPointer::set_status(const QString &status)
{
    ui->lbl_state->setText(status);
}

/**
 * @brief Enable or disable the form controls and pointer
 * @param state new state
 */
void SpatialPointer::set_enabled(const bool &state)
{
    enabled_ = state;
    enabled_ ? tmr_update->start(kUpdateRate) : tmr_update->stop();

    ui->btn_enable->setEnabled(state == false);
    ui->btn_disable->setEnabled(state == true);

    set_status(state ? kStatusWorking : kStatusIdle);
}

/**
 * \brief Move the mouse cursor relative to its current position
 * \param x x velocity
 * \param x y velocity
*/
void SpatialPointer::move_cursor(const int &x, const int &y)
{
    QPoint cursor_position = QCursor::pos();
    QCursor::setPos(cursor_position.x() + x, cursor_position.y() + y);
}

/**
 * @brief Enable button clicked event
 */
void SpatialPointer::on_btn_enable_clicked()
{
    if(!spatial_->attatched() && !spatial_->initialize(8, 1))
    {
        set_status(kStatusFail);
        return;
    }

    set_enabled(true);
}

/**
 * @brief Disable button clicked event
 */
void SpatialPointer::on_btn_disable_clicked()
{
    set_enabled(false);
}

/**
 * @brief Github button click event
 */
void SpatialPointer::on_btn_github_clicked()
{
    QDesktopServices::openUrl(QUrl(ui->btn_github->text()));
}

/**
 * @brief Deadzone slider value changed event
 * @param value new value
 */
void SpatialPointer::on_sld_deadzone_valueChanged(int value)
{
    tolerance_= value;
    ui->lbl_deadzone_value->setText(QString::number(value));
}

/**
 * @brief Speed slider value changed event
 * @param value new value
 */
void SpatialPointer::on_sld_speed_valueChanged(int value)
{
    speed_ = value;
    ui->lbl_speed_value->setText(QString::number(value));
}
