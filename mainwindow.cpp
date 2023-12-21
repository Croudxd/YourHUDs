#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "hud.h"
#include <iostream>
#include <QFile>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QMessageBox>
#include <QInputDialog>
#include "miniz/miniz.h"
#include <filesystem>
#include <fstream>
#include <QDebug>
#include <QFileDialog>
#include <QSettings>
#include <QTimer>


namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentHud(nullptr)
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
            if(!installPath.isNull()){
                if(writeHudTxt())
                {
                    QTimer::singleShot(5000, this, [&]() {
                        try
                            {
                            extractHud(installPath);

                } catch (const std::exception& e)
                    {
                            qDebug() << "Exception during extraction: " << e.what();
                        }
                    });
                }

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
                        if(writeHudTxt())
                        {
                            QTimer::singleShot(5000, this, [&]() {
                                try
                                {
                                    extractHud(installPath);

                                } catch (const std::exception& e)
                                {
                                    qDebug() << "Exception during extraction: " << e.what();
                                }
                            });
                        }

                    }
                    else
                    {
                        QMessageBox::warning(this, "Install/extraction error", "Error sending extract information");
                    }
                }
            }
        }
    }




QString MainWindow::installHud()
{
    QString downloadLink = currentHud->getDownloadLink();

    // Ask the user to select a location to save the file
    QString installPath = QFileDialog::getExistingDirectory(this, "Select directory to install", QDir::homePath());

    // Check if the user canceled the dialog
    if (installPath.isEmpty()) {
        qDebug() << "Installation canceled by the user.";
        return QString();
    }

    QString zipName = "downloaded_file.zip";
    // Create a network manager to handle the download
    QNetworkAccessManager manager;
    QNetworkRequest request(downloadLink);
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
    QString zipFilePath = installPath + "\\" + zipName;
    QFile zipFile(zipFilePath);
    if (zipFile.open(QIODevice::WriteOnly)) {
        zipFile.write(reply->readAll());
        zipFile.close();
        qDebug() << "Installation successful!";
        return zipFilePath;
        return QString();
    } else {
        qDebug() << "Error saving file to disk.";
        return QString();
    }

    // Clean up the network reply
    reply->deleteLater();
}


bool MainWindow::extractHud(const QString &zipFilePath)
{
    fs::path zipPath(zipFilePath.toStdString());
    std::string outputDirectory = zipPath.parent_path().string();

    // Read the ZIP file into memory
    std::ifstream zipFile(zipFilePath.toStdString(), std::ios::binary);
    if (!zipFile.is_open()) {
        std::cerr << "Error: ZIP file does not exist. Path: " << zipFilePath.toStdString() << std::endl;
        return false;
    }

    zipFile.seekg(0, std::ios::end);
    std::streampos zipFileSize = zipFile.tellg();
    zipFile.seekg(0, std::ios::beg);
    std::vector<char> zipData(zipFileSize);
    zipFile.read(zipData.data(), zipFileSize);

    // Initialize the miniz ZIP archive
    mz_zip_archive zipArchive;
    memset(&zipArchive, 0, sizeof(zipArchive));
    if (!mz_zip_reader_init_mem(&zipArchive, zipData.data(), static_cast<size_t>(zipFileSize), 0)) {
        std::cerr << "Error: Unable to initialize ZIP archive." << std::endl;
        return false;
    }

    // Get the number of files in the ZIP archive
    size_t numFiles = mz_zip_reader_get_num_files(&zipArchive);

    // Iterate through each file in the ZIP archive
    for (size_t i = 0; i < numFiles; ++i) {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zipArchive, i, &file_stat)) {
            std::cerr << "Error: Unable to get file information for file " << i << std::endl;
            continue;
        }

        // Extract each file to the output directory, preserving the directory structure
        fs::path outputPath = zipPath.parent_path() / fs::path(file_stat.m_filename);
        if (!mz_zip_reader_extract_to_file(&zipArchive, i, outputPath.string().c_str(), 0)) {
            std::cerr << "Error: Unable to extract file " << file_stat.m_filename << std::endl;
            continue;
        }
    }

    // Clean up
    mz_zip_reader_end(&zipArchive);

    return true;
}


bool MainWindow::uninstallHud(QString hudtxt)
{
    QDir hudDir(hudtxt);
    if (hudDir.exists())
    {
        hudDir.removeRecursively();
        if (!hudDir.exists())
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

bool MainWindow::writeHudTxt()
{
    QString filePath = "hud.txt";
    QFile pathFile(filePath);

    if (pathFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&pathFile);
        out << currentPath;
        out << "\\";
        out << currentHud->getHudFileName();
        pathFile.close();
        return true;
    }
    else
    {
        qDebug() << "Write Path Error: Could not update TF2 path in file.";
        return false;
    }
}

void MainWindow::writePathTxt()
{

    QString filePath = "path.txt";
    QFile pathFile(filePath);
    if (pathFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&pathFile);
        out << currentPath;
        pathFile.close();
    }
    else
    {
        qDebug() << "Write Path Error: Could not update TF2 path in file.";
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


void MainWindow::on_actionOptions_triggered()
{
    QInputDialog getTf2Path;
    getTf2Path.setWindowTitle("Options");
    getTf2Path.setLabelText("Enter TF2 custom path: ");
    getTf2Path.setStyleSheet("* {color:black }");
    if (getTf2Path.exec() == QDialog::Accepted)
    {
        currentPath = getTf2Path.textValue();
        writePathTxt();
    }
}



