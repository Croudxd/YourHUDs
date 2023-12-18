#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "hud.h"
#include <iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->button7Hud, SIGNAL(clicked()), this, SLOT(on_button7Hud_clicked()));

}
MainWindow::~MainWindow()
{
    delete currentHud;
    delete ui;
}

void MainWindow::on_installbutton_clicked()
{
    QString link = currentHud->getDownloadLink();
}

std::vector<QString> MainWindow::createImagesVector(QString fileName)
{

    std::vector<QString> imagePaths;
    //retreieve images from hud file.
    QString path = ":/imgs/";
    path += fileName + "/";

    for (int i = 1; i < 5 ; i++)
    {
        QString imgPath = path + "image" + QString::number(i) + ".jpg";
        imagePaths.push_back(imgPath);
    }

    return imagePaths;
}





void MainWindow::setHud(QString name, QString description, QString creator, QString downloadlink){
    currentHud = new hud();
    currentHud->setName(name);
    currentHud->setDescription(description);
    currentHud->setCreator(creator);
    currentHud->setDownloadLink(downloadlink);
    ui->textBrowser->setText("Name: " + currentHud->getName() + "\nDescription: " + currentHud->getDescription() + "\nCreator: " + currentHud->getCreator());
}
void MainWindow::setImages(QLabel *label, const QString imgPath)
{
    QPixmap pixmap(imgPath);
    label->setPixmap(pixmap);
    label->setScaledContents(true);
}



void MainWindow::on_button7Hud_clicked()
{

    setHud("FlawHUD", "FlawHUD Custom hud", "CriticalFlaw", "https://github.com/CriticalFlaw/flawhud");
    std::cout << "Calling createImagesVector" << std::endl;
    std::vector<QString> imgPaths = createImagesVector("FlawHUD");
    setImages(ui->label_2, imgPaths[0]);
    currentHud->setImages(imgPaths);
    for(const QString &img : imgPaths){
        qDebug() << img;
    }


}
void MainWindow::on_buttonBudHUD_clicked()
{
    setHud("BudHUD", "BudHUD Custom HUD", "rbJaxter", "https://github.com/rbjaxter/budhud");
}





void MainWindow::on_leftImageButton_clicked()
{
    std::vector<QString> paths = currentHud->getImages();
    int index = currentHud->getImageNumber();

    if (index > 0 && index < paths.size())
    {
        index--;
        currentHud->setImageNumber(index);
        setImages(ui->label_2, paths[index]);
    }
}
void MainWindow::on_rightImageButton_clicked()
{
    std::vector<QString> paths = currentHud->getImages();
    int index = currentHud->getImageNumber();

    if (index >= 0 && index < paths.size() - 1)
    {
        index++;
        currentHud->setImageNumber(index);
        setImages(ui->label_2, paths[index]);
    }
}

