#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "hud.h"
#include <QFile>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include <QFileDialog>
#include <QSettings>
#include <QTimer>
#include <QProcess>
#include <QPointer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentHud(nullptr)
    , currentCustomHud()
    , customHUDs()

{
    ui->setupUi(this);
    this->setFixedWidth(800);
    this->setFixedHeight(600);
    QMenuBar *q = this->menuBar();
    q->setStyleSheet("color: grey");
    setWindowIcon(QIcon(":/logo/logo.png"));
}


MainWindow::~MainWindow()
{
    delete currentHud;
    delete ui;
}

void MainWindow::on_installbutton_clicked()
{
        QString hudtxt = readHudTxt();
        if(hudtxt.isNull()){
            QString installPath = installHud();
            if(!installPath.isNull())
            {
                extractHud(installPath);
            }
            else
            {
                QMessageBox::warning(this, "Install/extraction error", "Error sending extract information");
            }
        }
        else
        {
            if(!hudtxt.isNull())
            {
                if(uninstallHud(hudtxt))
                {
                    QString installPath = installHud();
                    if(!installPath.isNull())
                    {
                        extractHud(installPath);
                        }
                    }
                    else
                    {
                        QMessageBox::warning(this, "Install/extraction error", "Error sending extract information");
                    }
                }
            }
        }





QString MainWindow::installHud()
{

    if (currentHud->getDownloadLink().isEmpty()) {
        qDebug() << "Download link is empty.";
        return QString();
    }

    QString pathtxt = readPathTxt();
    if(!pathtxt.isEmpty())
    {

        QString zipFilePath = QDir(pathtxt).filePath("downloaded_file.zip");


        // Create a network manager to handle the download
        QNetworkAccessManager manager;
        QNetworkRequest request(currentHud->getDownloadLink());
        QNetworkReply* reply = manager.get(request);

        // Wait for the download to finish
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        // Check if the download was successful
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Error downloading file: " << reply->errorString();
            reply->deleteLater();
            return QString();
        }

        // Save the downloaded data to the selected file
        QFile zipFile(zipFilePath);
        if (zipFile.open(QIODevice::WriteOnly)) {
            zipFile.write(reply->readAll());
            zipFile.close();
            qDebug() << "Installation successful!";
            QString absoluteZipFilePath = QFileInfo(zipFile).absoluteFilePath();
            QString installedHudPath = QDir(pathtxt).filePath(currentHud->getHudFileName());
            if (writeHudTxt(installedHudPath)) {
                qDebug() << "HUD path written to hud.txt successfully.";
            } else {
                qDebug() << "Error writing HUD path to hud.txt.";
            }
            return absoluteZipFilePath;
        } else {
            qDebug() << "Error saving file to disk.";
            return QString();
        }

        // Clean up the network reply
        reply->deleteLater();
    }
    else
    {
    // Get the file path to save
    QString downloadDir = QFileDialog::getExistingDirectory(this, "Select Download Directory", QDir::homePath());

    // Check if the user canceled the dialog
    if (downloadDir.isEmpty()) {
        qDebug() << "Installation canceled by the user.";
        return QString();
    }

    QString zipFilePath = QDir(downloadDir).filePath("downloaded_file.zip");


    // Create a network manager to handle the download
    QNetworkAccessManager manager;
    QNetworkRequest request(currentHud->getDownloadLink());
    QNetworkReply* reply = manager.get(request);

    // Wait for the download to finish
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Check if the download was successful
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Error downloading file: " << reply->errorString();
        reply->deleteLater();
        return QString();
    }

    // Save the downloaded data to the selected file
    QFile zipFile(zipFilePath);
    if (zipFile.open(QIODevice::WriteOnly)) {
        zipFile.write(reply->readAll());
        zipFile.close();
        qDebug() << "Installation successful!";
        QString absoluteZipFilePath = QFileInfo(zipFile).absoluteFilePath();
        QString installedHudPath = QDir(downloadDir).filePath(currentHud->getHudFileName());
        if (writeHudTxt(installedHudPath)) {
            qDebug() << "HUD path written to hud.txt successfully.";
        } else {
            qDebug() << "Error writing HUD path to hud.txt.";
        }
        if(writePathTxt(downloadDir)){
            return absoluteZipFilePath;
        }
        else
        {
            qDebug() << "Failed to write to pathtxt";
        }
    } else {
        qDebug() << "Error saving file to disk.";
        return QString();
    }

    // Clean up the network reply
    reply->deleteLater();
    }
    return QString();
}


bool MainWindow::extractHud(const QString &zipFilePath)
{

    QString zipPath = zipFilePath;
    QStringList param;
    param << "unzip.py" << zipPath;  // Adjust the order of parameters

    QPointer<QProcess> q = new QProcess;
    q->setProgram("python");  // Set the program to "python"
    q->setArguments(param);
    q->setProcessChannelMode(QProcess::MergedChannels);
    q->start();
    if(q->waitForFinished(-1)){
        qDebug() << "Process finished success";
        // Read and display standard output
        qDebug() << "Output: " << q->readAllStandardOutput();
        if (QFile::remove(zipFilePath)) {
            qDebug() << "Original ZIP file deleted successfully.";
        } else {
            qDebug() << "Error deleting the original ZIP file.";
        }

        return true;
    }
    else
    {
        qDebug() << "Error occured waiting for process to finish";
    }


    return false;
}
bool MainWindow::uninstallHud(QString hudtxt)
{
    QDir hudDir(hudtxt);
    if (hudDir.exists())
    {
        hudDir.removeRecursively();
        if (hudDir.exists())
        {
            QMessageBox::critical(this, "Uninstall Error", "Failed to uninstall HUD. Please ensure the directory is not in use.");
            return false;
        }
        else
        {
            QMessageBox::information(this, "Uninstall Successful", "HUD uninstalled successfully.");
            return true;
        }
    }
    else
    {
        QMessageBox::warning(this, "Uninstall Error", "The specified directory does not exist.");
        return false;

    }
}


QString MainWindow::readHudTxt()
{
    QString filePath = "hud.txt";
    QFile hudFile(filePath);

    if (!hudFile.exists())
    {
        qDebug() << "Read Hud Error: " << filePath << " does not exist.";
        return QString();
    }

    if (hudFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&hudFile);
        QString content = in.readAll();
        hudFile.close();
        if(content.isNull())
        {
            qDebug() << "Read Hud Error: hud.txt is empty";
            return QString();
        }
        return content;
    }
    else
    {
        qDebug() << "Read Hud Error: Could not open" << filePath << " for reading.";
        return QString();
    }
}


QString MainWindow::readPathTxt()
{
    QString filePath = "path.txt";
    QFile hudFile(filePath);
    if (hudFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&hudFile);
        QString content = in.readAll();
        hudFile.close();
        return content;
    }
    else
    {
        qDebug() << "Read Path Error: Could not open path.txt for reading.";
        return QString();
    }
}

bool MainWindow::writeHudTxt(QString installPath)
{
    QString filePath = "hud.txt";
    QFile pathFile(filePath);

    if (currentHud && pathFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&pathFile);
        out << installPath;
        pathFile.close();
        return true;
    }
    else
    {
        qDebug() << "Write Path Error: Could not update TF2 path in file.";
        return false;
    }
}

bool MainWindow::writePathTxt(QString &installPath)
{

    QString filePath = "path.txt";
    QFile pathFile(filePath);
    if (pathFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&pathFile);
        out << installPath;
        pathFile.close();
        return true;
    }
    else
    {
        qDebug() << "Write Path Error: Could not update TF2 path in file.";
        return false;
    }
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


void MainWindow::setHud(QString name, QString description, QString creator, QString downloadlink, QString hudFileName){
    if (currentHud)
    {
        delete currentHud;
        currentHud = nullptr;
    }
    currentHud = new hud();
    currentHud->setName(name);
    currentHud->setDescription(description);
    currentHud->setCreator(creator);
    currentHud->setDownloadLink(downloadlink);
    currentHud->setHudFileName(hudFileName);
    ui->textBrowser->setText("Name: " + currentHud->getName() + "\nDescription: " + currentHud->getDescription() + "\nCreator: " + currentHud->getCreator());
}

void MainWindow::setImages(QLabel *label, const QString imgPath)
{
    label->clear();
    QPixmap pixmap(imgPath);
    if (pixmap.isNull())
    {
        qDebug() << "Image loading failed!";
    }
    else
    {
        qDebug() << "Image loaded successfully.";
    }
    label->setPixmap(pixmap);
    label->setScaledContents(true);
    qDebug() << "Image Path:" << imgPath;

}
void MainWindow::on_leftImageButton_clicked()
{
    std::vector<QString> paths = currentHud->getImages();
    int index = currentHud->getImageNumber();
    qDebug() << "Left Image Button Clicked!";
    if (index > 0)
    {
        index--;
    }
    else
    {
        index = paths.size() - 1;
    }

    currentHud->setImageNumber(index);

    if (index >= 0 && index < paths.size())
    {
        setImages(ui->label_2, paths[index]);
    }
    else
    {
        qDebug() << "Left Image Button Clicked! Error: Index out of bounds.";
    }
    currentHud->setImageNumber(index);
    setImages(ui->label_2, paths[index]);
}

void MainWindow::on_rightImageButton_clicked()
{
    std::vector<QString> paths = currentHud->getImages();
    int index = currentHud->getImageNumber();
    qDebug() << "Right Image Button Clicked!";
    if(currentHud == nullptr)
    {

    }
    else
    {

    }
    if (index < paths.size() - 1)
    {
        index++;
    }
    else
    {
        index = 0;
    }

    currentHud->setImageNumber(index);

    if (index >= 0 && index < paths.size())
    {
        setImages(ui->label_2, paths[index]);
    }
    else
    {
        qDebug() << "Right Image Button Clicked! Error: Index out of bounds.";
    }

    currentHud->setImageNumber(index);
    setImages(ui->label_2, paths[index]);
}



//ALL HUD BUTTONS
void MainWindow::on_button7Hud_clicked()
{
    setHud("FlawHUD", "FlawHUD Custom hud", "CriticalFlaw", "https://github.com/CriticalFlaw/flawhud/releases/download/2023.1013/flawhud.zip", "flawhud");
    std::vector<QString> imgPaths = createImagesVector("FlawHUD");
    setImages(ui->label_2, imgPaths[0]);
    currentHud->setImages(imgPaths);
}
void MainWindow::on_buttonBudHUD_clicked()
{
    setHud("BudHUD", "BudHUD Custom HUD", "rbJaxter", "https://github.com/rbjaxter/budhud/releases/download/2312_01/budhud-master.zip", "budhud-master");
    std::vector<QString> imgPaths = createImagesVector("BudHUD");
    setImages(ui->label_2, imgPaths[0]);
    currentHud->setImages(imgPaths);
}
void MainWindow::on_buttonpeachHUD_clicked()
{
    setHud("PeachHUD", "PeachHUD Custom hud", "Peach", "https://github.com/PapaPeach/PeachHUD/archive/refs/tags/v3.3.2.zip", "PeachHUD-3.3.2");
    std::vector<QString> imgPaths = createImagesVector("PeachHUD");
    setImages(ui->label_2, imgPaths[0]);
    currentHud->setImages(imgPaths);
}
void MainWindow::on_buttonzeHUD_clicked()
{
    setHud("zeHUD", "zeHUD Custom hud", "bo0bsy", "https://github.com/bo0bsy/zehud/archive/refs/tags/v1.0.zip", "zehud-1.0");
    std::vector<QString> imgPaths = createImagesVector("zeHUD");
    setImages(ui->label_2, imgPaths[0]);
    currentHud->setImages(imgPaths);
}
void MainWindow::on_oneHUD_clicked()
{
    setHud("OneHUD", "OneHUD Custom hud", "creator", "https://github.com/leadscales/onehud/archive/refs/tags/v0.9.2.zip", "onehud-0.9.2");
    std::vector<QString> imgPaths = createImagesVector("oneHUD");
    setImages(ui->label_2, imgPaths[0]);
    currentHud->setImages(imgPaths);
}

//EDIT THESE BUTTON FUNCTIONS BELOW TO ADD CUSTOM HUD BUTTONS.
void MainWindow::on_buttonHUD_clicked()
{
    // setHud("OneHUD", "OneHUD Custom hud", "creator", "https://github.com/leadscales/onehud/archive/refs/tags/v0.9.2.zip", "onehud-0.9.2");
    // std::vector<QString> imgPaths = createImagesVector("oneHUD");
    // setImages(ui->label_2, imgPaths[0]);
    // currentHud->setImages(imgPaths);
}
void MainWindow::on_buttonHUD2_clicked()
{
    // setHud("OneHUD", "OneHUD Custom hud", "creator", "https://github.com/leadscales/onehud/archive/refs/tags/v0.9.2.zip", "onehud-0.9.2");
    // std::vector<QString> imgPaths = createImagesVector("oneHUD");
    // setImages(ui->label_2, imgPaths[0]);
    // currentHud->setImages(imgPaths);
}
void MainWindow::on_buttonHUD3_clicked()
{
    // setHud("OneHUD", "OneHUD Custom hud", "creator", "https://github.com/leadscales/onehud/archive/refs/tags/v0.9.2.zip", "onehud-0.9.2");
    // std::vector<QString> imgPaths = createImagesVector("oneHUD");
    // setImages(ui->label_2, imgPaths[0]);
    // currentHud->setImages(imgPaths);
}
void MainWindow::on_buttonHUD4_clicked()
{
    // setHud("OneHUD", "OneHUD Custom hud", "creator", "https://github.com/leadscales/onehud/archive/refs/tags/v0.9.2.zip", "onehud-0.9.2");
    // std::vector<QString> imgPaths = createImagesVector("oneHUD");
    // setImages(ui->label_2, imgPaths[0]);
    // currentHud->setImages(imgPaths);
}
void MainWindow::on_buttonHUD5_clicked()
{
    // setHud("OneHUD", "OneHUD Custom hud", "creator", "https://github.com/leadscales/onehud/archive/refs/tags/v0.9.2.zip", "onehud-0.9.2");
    // std::vector<QString> imgPaths = createImagesVector("oneHUD");
    // setImages(ui->label_2, imgPaths[0]);
    // currentHud->setImages(imgPaths);
}
void MainWindow::on_buttonHUD6_clicked()
{
    // setHud("OneHUD", "OneHUD Custom hud", "creator", "https://github.com/leadscales/onehud/archive/refs/tags/v0.9.2.zip", "onehud-0.9.2");
    // std::vector<QString> imgPaths = createImagesVector("oneHUD");
    // setImages(ui->label_2, imgPaths[0]);
    // currentHud->setImages(imgPaths);
}

//CUSTOM HUD FUNCTIONS
//CUSTOM HUD FUNCTIONS
//CUSTOM HUD FUNCTIONS
//CUSTOM HUD FUNCTIONS
//CUSTOM HUD FUNCTIONS

void MainWindow::plus()
{
    //calls all functions belowg that need to be used within the add button.
    addButton();
}

bool MainWindow::addButton()
{
    QString downloadDir = QFileDialog::getExistingDirectory(this, "Select HUD Directory (Make sure that you select inside the HUD directory).", QDir::homePath());
    currentCustomHud.setPath(downloadDir);
    QString hudName = QInputDialog::getText(this,"Enter HUD name","Enter name of HUD/Button.");
    currentCustomHud.setName(hudName);
    customHUDs.push_back(currentCustomHud);
    // Find the widget containing the grid layout
    QWidget* gridLayoutWidget = ui->widget;

    // Find the layout inside the widget
    QGridLayout* layout = gridLayoutWidget->findChild<QGridLayout*>("gridLayout");

    // Check if the layout is valid
    if (!layout) {
        qDebug() << "Error: Unable to find gridLayout.";
        return false;
    }

    // Create a new button
    QPushButton* button = new QPushButton(hudName, gridLayoutWidget);
    button->setStyleSheet("color: rgb(0, 0, 0); background-color: rgb(179, 179, 179);");

    int row = layout->count() / 3; // Assuming 3 columns
    int col = layout->count() % 3;

    // Add the button to the layout at the specified row and column
    layout->addWidget(button, row, col);

    connect(button, &QPushButton::clicked, [=]() {
        int buttonIndex = layout->indexOf(button);

        // Retrieve the corresponding CustomHud object
        if (buttonIndex >= 0 && buttonIndex < customHUDs.size()) {
            const customhud& clickedHud = customHUDs.at(buttonIndex);
            QString pathtxt = readPathTxt();
            if(!pathtxt.isEmpty()){
                // Perform the desired function with the clickedHud
                qDebug() << "Button clicked: " << clickedHud.getName();
                QString hudFilePath = clickedHud.getPath();
                QString downloadFilePath = pathtxt;
                copyHud(hudFilePath, downloadFilePath);
            }
            else
            {
                qDebug() << "Button clicked: " << clickedHud.getName();
                QString hudFilePath = clickedHud.getPath();
                QString downloadFilePath = QFileDialog::getExistingDirectory(this, "Select Custom Folder", QDir::homePath());
                copyHud(hudFilePath, downloadFilePath);
                writePathTxt(downloadFilePath);
            }
        }
    });

    // Add the button to the layout
    layout->addWidget(button);

    // Ensure that the layout updates
    gridLayoutWidget->adjustSize();

    return true;

    //dynamically add a button, from the vector 1-6;
}

bool MainWindow::loadButtonsFromData()
{
    //funciton to load data from the customHUDs vector and turn them into buttons.
    QString content = readHudFile();
    if(content.isNull())
    {
        return false;
    }
    else
    {
        //create buttons with objects within CustomHUDs vector.
    }
}

bool MainWindow::removeButton()
{
    REMOVE=true;
    //Set's remove to true
    //another click will set it too false.
    return true;
}

bool MainWindow::copyHud(QString src, QString dst)
{
    qDebug() << "copyHud executes.";
    qDebug() << "Copying from: " << src << " to: " << dst;
    QDir dir(src);
    if (!dir.exists())
    {
        qDebug() << "Source directory does not exist: " << src;
        return false;
    }

    // Create destination directory if it doesn't exist
    if (!QDir(dst).exists() && !QDir().mkpath(dst))
    {
        qDebug() << "Failed to create destination directory: " << dst;
        return false;
    }

    foreach (const QString& d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        QString src_path = src + QDir::separator() + d;
        QString dst_path = dst + QDir::separator() + d;

        // Recursive call to copy subdirectories
        if (!copyHud(src_path, dst_path))
        {
            qDebug() << "Failed to copy subdirectory: " << src_path;
            return false;
        }
    }

    foreach (const QString& f, dir.entryList(QDir::Files))
    {
        // Copy files
        if (!QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f))
        {
            qDebug() << "Failed to copy file: " << src + QDir::separator() + f;
            return false;
        }
    }

    return true;
}

bool MainWindow::toFile()
{
    QString content;
    for(customhud &hud : customHUDs)
    {
        content + hud.getName();
        content + " ";
        content + hud.getPath();
        content + "; ";
    }

    //save content to custom.txt file
    QString filePath = "custom.txt";
    QFile pathFile(filePath);
    if (pathFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&pathFile);
        out << content;
        pathFile.close();
        return true;
    }
    else
    {
        qDebug() << "Write Path Error: Could not update TF2 path in file.";
        return false;
    }
}

QString MainWindow::readHudFile()
{
    //read a txt file, each object will be seperated by a ; and the name and the path will be seperated by a path.
    //Split these by the values and then create objects with any huds that have already been made.
    QString filePath = "custom.txt";
    QFile hudFile(filePath);
    if (hudFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&hudFile);
        QString content = in.readAll();
        hudFile.close();
        //Manipulate all data splitting it into new arrays by ; and each index of array split by " ";
        QStringList hudList = content.split(";");
        for(const QString& hud : hudList)
        {
            QStringList hudInfo = hud.trimmed().split(" ");

            if(hudInfo.size() == 2){
                QString hudName = hudInfo[0];
                QString hudPath = hudInfo[1];

                customhud newhud;

                newhud.setName(hudName);
                newhud.setPath(hudPath);
                customHUDs.push_back(newhud);
            }
        }
        //create new customhud objects, add them to customHUDs and then add buttons to each.
        return content;
    }
    else
    {
        qDebug() << "Read Path Error: Could not open path.txt for reading.";
        return QString();
    }
}


//Add and remove buttons.


void MainWindow::on_addButton_clicked()
{
    plus();
}


void MainWindow::on_removeButton_clicked()
{
    removeButton();
}

