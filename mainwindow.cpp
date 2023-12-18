#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "hud.h"
#include <iostream>
#include <QFile>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->button7Hud, &QPushButton::clicked, this, &MainWindow::on_button7Hud_clicked);

}
MainWindow::~MainWindow()
{
    delete currentHud;
    delete ui;
}

void MainWindow::on_installbutton_clicked()
{
    QString link = currentHud->getDownloadLink();

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(link));

    QNetworkReply* reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();

            // Save the downloaded data to a file
            QString fileName = "downloaded_file.zip";  // Set your desired file name
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(data);
                file.close();
                qDebug() << "File downloaded successfully: " << fileName;
            } else {
                qDebug() << "Failed to save file.";
            }
        } else {
            qDebug() << "Download failed: " << reply->errorString();
        }

        reply->deleteLater();
        manager->deleteLater();
    });
}

std::vector<QString> MainWindow::createImagesVector(QString fileName)
{
    std::vector<QString> imagePaths;
    QString path = ":/imgs/imgs/" + fileName + "/";

    for (int i = 1; i <= 4; ++i)
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
    label->clear();
    QPixmap pixmap(imgPath);
    if (pixmap.isNull()) {
        qDebug() << "Image loading failed!";
    } else {
        qDebug() << "Image loaded successfully.";
    }
    label->setPixmap(pixmap);
    label->setScaledContents(true);
}



void MainWindow::on_button7Hud_clicked()
{
    std::cout << "Hello" << std::endl;
    setHud("FlawHUD", "FlawHUD Custom hud", "CriticalFlaw", "https://github.com/CriticalFlaw/flawhud");
    std::vector<QString> imgPaths = createImagesVector("FlawHUD");
    setImages(ui->label_2, imgPaths[0]);
    currentHud->setImages(imgPaths);
}
void MainWindow::on_buttonBudHUD_clicked()
{
    setHud("BudHUD", "BudHUD Custom HUD", "rbJaxter", "https://github.com/rbjaxter/budhud");
    std::vector<QString> imgPaths = createImagesVector("BudHUD");
    setImages(ui->label_2, imgPaths[0]);
    currentHud->setImages(imgPaths);
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

