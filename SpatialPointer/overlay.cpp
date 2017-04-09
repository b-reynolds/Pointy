#include "overlay.h"
#include "ui_overlay.h"
#include <QTimer>

Overlay::Overlay(QWidget *parent) : QWidget(parent), ui(new Ui::Overlay)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    setParent(0); // Create TopLevel-Widget
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_PaintOnScreen); // not needed in Qt 5.2 and up

    // Initialize and connect the update timer to the update function
    tmr_update = new QTimer(this);
    connect(tmr_update, SIGNAL(timeout()), this, SLOT(slot_update()));


    set_countdown(kCountdownTime);
}

Overlay::~Overlay()
{
    delete ui;
}

void Overlay::set_enabled(const bool& state, const int& countdown)
{
    set_countdown(countdown);

    if(state)
    {
        tmr_update->start(1000);
        show();
        return;
    }

    tmr_update->stop();
    hide();
}

void Overlay::slot_update()
{
    if(countdown_ > 1)
        set_countdown(countdown_ - 1);
    else
    {
        set_enabled(false, countdown_);
    }
}

void Overlay::set_countdown(const int& value)
{
    countdown_ = value;
    ui->lbl_countdown->setText(QString::number(countdown_));
}
