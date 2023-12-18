#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    //hud = (name, creator, images, etc etc.)
    //possibly need a function for the images. - create resource folder, then have images folder, then name of the hud ie 7HUD which olds pictures. gets all pictures creates objects of them and saves to a vector returns vector.
    //get functions and store info.
    //then call change functions to change the information
    // calls destructor.
}



