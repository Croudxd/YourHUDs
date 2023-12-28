#include "customwindow.h"
#include "ui_customwindow.h"

customwindow::customwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::customwindow)
{
    ui->setupUi(this);
}

customwindow::~customwindow()
{
    delete ui;
}
