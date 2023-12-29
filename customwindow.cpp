#include "customwindow.h"
#include "ui_customwindow.h"
#include <QMenuBar>

customwindow::customwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::customwindow)
{
    ui->setupUi(this);
    this->setFixedWidth(800);
    this->setFixedHeight(600);
    setWindowIcon(QIcon(":/logo/logo.png"));
    connect(ui->pushButton, &QPushButton::clicked, this, &customwindow::on_pushButton_2_clicked);

}

customwindow::~customwindow()
{
    delete ui;
}

void customwindow::on_pushButton_2_clicked()
{
    this->hide();
    if (parentWidget()) {
        parentWidget()->show();
    }}

