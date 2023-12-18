#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "hud.h"

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

    for (int i = 1; i < 5; i++)
    {
        QString imgPath = path + "image" + QString::number(i) + ".jpg";
        imagePaths.push_back(imgPath);
    }

    return imagePaths;
}





void MainWindow::setHud(QString name, QString description, QString creator, QString downloadlink){
    std::vector<QImage> images;
    currentHud = new hud();
    currentHud->setName(name);
    currentHud->setDescription(description);
    currentHud->setCreator(creator);
    currentHud->setDownloadLink(downloadlink);
    std::vector<QString> imgPaths = createImagesVector("FlawHUD");
    setImages(ui->label_2, imgPaths[0]);
    currentHud->setImages(imgPaths);
    ui->textBrowser->setText("Name: " + currentHud->getName() + "\nDescription: " + currentHud->getDescription() + "\nCreator: " + currentHud->getCreator());
}
void MainWindow::setImages(QLabel *label, const QString imgPath)
{
    // Load the image from file
    QPixmap pixmap(imgPath);

    // Set the pixmap to the label
    label->setPixmap(pixmap);

    // Optionally, scale the image to fit the label
    label->setScaledContents(true);
}



void MainWindow::on_button7Hud_clicked()
{
    setHud("FlawHUD", "FlawHUD Custom hud", "CriticalFlaw", "https://github.com/CriticalFlaw/flawhud");
    std::vector<QString> imgPaths = createImagesVector("FlawHUD");
    setImages(ui->label_2, imgPaths[0]);


}
void MainWindow::on_buttonBudHUD_clicked()
{
    setHud("BudHUD", "BudHUD Custom HUD", "rbJaxter", "https://github.com/rbjaxter/budhud");
}





void MainWindow::on_leftImageButton_clicked()
{
    std::vector<QString> paths = currentHud->getImages();
    int index = currentHud->getImageNumber();
    if (index == 0)
    {
        return;
    } else{
        index --;
        currentHud->setImageNumber(index);
        setImages(ui->label_2, paths[0]);
        currentHud->setImages(paths);
    }
}


void MainWindow::on_rightImageButton_clicked()
{
    std::vector<QString> paths = currentHud->getImages();
    int index = currentHud->getImageNumber();
    if (index == 4)
    {
        return;
    } else{
        index ++;
        currentHud->setImageNumber(index);
        setImages(ui->label_2, paths[0]);
        currentHud->setImages(paths);
    }
}

