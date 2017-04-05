#include "overlay.h"
#include "ui_overlay.h"
#include <QTimer>

Overlay::Overlay(QWidget *parent) : QWidget(parent), ui(new Ui::Overlay)
{
    ui->setupUi(this);

    QWidget::setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    //QWidget::setAttribute(Qt::WA_PaintOnScreen);

    // Initialize and connect the update timer to the update function
    tmr_update = new QTimer(this);
    connect(tmr_update, SIGNAL(timeout()), this, SLOT(slot_update()));


    set_countdown(kCountdownTime);
}

Overlay::~Overlay()
{
    delete ui;
}

void Overlay::set_enabled(const bool& state)
{
    if(state)
    {
        tmr_update->start(1000);
        show();

        return;
    }

    tmr_update->stop();
    set_countdown(kCountdownTime);
    hide();
}

void Overlay::slot_update()
{
    if(countdown_ > 0)
        set_countdown(countdown_ - 1);
    else
    {
        set_enabled(false);
    }
}

void Overlay::set_countdown(const int& value)
{
    countdown_ = value;
    ui->lbl_countdown->setText(QString::number(countdown_));
}
