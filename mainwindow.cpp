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
#include <miniz.h>





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentHud(nullptr)
{
    ui->setupUi(this);
    this->setFixedWidth(800);
    this->setFixedHeight(600);
    QMenuBar *q = this->menuBar();
    q->setStyleSheet("color: white");
}
MainWindow::~MainWindow()
{
    delete currentHud;
    delete ui;
}

void MainWindow::on_installbutton_clicked()
{
    QString pathtxt = readPathTxt();
    if(pathtxt.isNull())
    {
        QMessageBox::warning(this, "No TF2 path set", "Set a TF2 path in options to install HUD");
    } else
    {
        QString hudtxt = readHudTxt();
        if(hudtxt.isNull()){
            installFunction();
            writeHudTxt();
        } else
        {
            uninstallHud(hudtxt);
            installFunction();
            writeHudTxt();
        }
    }
}

void MainWindow::installFunction ()
 {
    if (currentPath.isNull()) {
        QMessageBox::warning(this, "No TF2 path set", "Set a TF2 path in options to install HUD");
        return;
    }

    QString link = currentHud->getDownloadLink();

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl(link));

    QNetworkReply* reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            // Save the downloaded file
            QByteArray data = reply->readAll();
            QString zipFilePath = currentPath + "/downloaded_file.zip";

            QFile file(zipFilePath);
            if (file.open(QIODevice::WriteOnly)) {
                qint64 bytesWritten = file.write(data);
                file.close();

                if (bytesWritten == data.size()) {
                    QMessageBox::information(this, "Installation Successful", "File downloaded successfully to: " + zipFilePath);

                    // Now you can extract the zip file using your extraction function
                    extractHud(zipFilePath);
                } else {
                    QMessageBox::critical(this, "Error", "Error writing to file. Bytes written: " + QString::number(bytesWritten));
                    QFile::remove(zipFilePath);  // Remove the incomplete file
                }
            } else {
                QMessageBox::critical(this, "Error", "Could not open file for writing. Error: " + file.errorString());
            }
        } else {
            QMessageBox::critical(this, "Error", "Download failed: " + reply->errorString());
        }

        // Clean up resources
        reply->deleteLater();
        manager->deleteLater();
    });
}


 bool MainWindow::extractHud(QString installPath) const
{
     std::string zipFilePath = installPath.toStdString();

     // Open the zip file
     mz_zip_archive zip;
     mz_bool success = mz_zip_reader_init_file(&zip, zipFilePath.c_str(), 0);

     if (!success)
     {
         qDebug() << "Failed to open the zip file";
         return false;
     }

     // Get the number of files in the zip archive
     size_t numFiles = mz_zip_reader_get_num_files(&zip);

     // Extract each file
     for (size_t i = 0; i < numFiles; ++i)
     {
         mz_zip_archive_file_stat file_stat;
         if (!mz_zip_reader_file_stat(&zip, i, &file_stat))
         {
             qDebug() << "Failed to get file stat";
             mz_zip_reader_end(&zip);
             return false;
         }

         // Allocate memory for file data
         void* fileData = malloc(file_stat.m_uncomp_size);

         // Read file data
         if (mz_zip_reader_extract_to_mem_no_alloc(
                 &zip, i, fileData, file_stat.m_uncomp_size, 0, nullptr, 0) != MZ_TRUE)
         {
             qDebug() << "Failed to extract file" << file_stat.m_filename;
             free(fileData);
             mz_zip_reader_end(&zip);
             return false;
         }

         // Save the extracted file data to disk (you can customize the path as needed)
         QString extractedFilePath = QString::fromStdString(file_stat.m_filename);
         QFile extractedFile(extractedFilePath);
         if (extractedFile.open(QIODevice::WriteOnly))
         {
             extractedFile.write(static_cast<const char*>(fileData), file_stat.m_uncomp_size);
             extractedFile.close();
         }

         // Free allocated memory
         free(fileData);
     }

     // Close the zip archive
     mz_zip_reader_end(&zip);

     // Remove the zip file after extraction
     QFile::remove(installPath);

     return true;
}

 void MainWindow::uninstallHud(QString hudtxt){
     QDir hudDir(hudtxt);

     // Check if the directory exists
     if (hudDir.exists()) {
         // Remove all files and subdirectories within the directory
         hudDir.removeRecursively();

         // Check if the removal was successful
         if (hudDir.exists()) {
             QMessageBox::critical(this, "Error", "Failed to uninstall HUD. Please ensure the directory is not in use.");
         } else {
             QMessageBox::information(this, "Uninstall Successful", "HUD uninstalled successfully.");
         }
     } else {
         QMessageBox::warning(this, "Directory Not Found", "The specified directory does not exist.");
     }
 }


QString MainWindow::readHudTxt()
{
    QString filePath = "hud.txt";  // Adjust the filename as needed
    QFile hudFile(filePath);

    if (hudFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&hudFile);
        QString content = in.readAll();
        hudFile.close();
        return content;
    } else {
        qDebug() << "Error: Could not open hud.txt for reading.";
        return QString();  // Return an empty string or some other indication of failure
    }
}


QString MainWindow::readPathTxt()
{
    QString filePath = "path.txt";  // Adjust the filename as needed
    QFile hudFile(filePath);

    if (hudFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&hudFile);
        QString content = in.readAll();
        hudFile.close();
        return content;
    } else {
        qDebug() << "Error: Could not open path.txt for reading.";
        return QString();  // Return an empty string or some other indication of failure
    }
}

void MainWindow::writeHudTxt()
{
    QString filePath = "hud.txt";  // Adjust the filename as needed
    QFile pathFile(filePath);

    if (pathFile.exists()) {
        // The file exists, open it in write mode to update the content
        if (pathFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&pathFile);
            currentPath.append(QChar('\\'));;
            currentPath.append(currentHud->getHudFileName());
            qDebug() << currentHud->getHudFileName();
            pathFile.close();

            qDebug() << "TF2 HUD path updated in file: " << currentPath;
        } else {
            qDebug() << "Error: Could not update TF2 path in file.";
        }
    } else {
        // The file doesn't exist, create a new file and save currentPath into it
        if (pathFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&pathFile);
            out << currentPath;
            pathFile.close();

            qDebug() << "TF2 path saved to new file: " << currentPath;
        } else {
            qDebug() << "Error: Could not save TF2 path to file.";
        }
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
    if (currentHud) {
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
    if (pixmap.isNull()) {
        qDebug() << "Image loading failed!";
    } else {
        qDebug() << "Image loaded successfully.";
    }
    label->setPixmap(pixmap);
    label->setScaledContents(true);
    qDebug() << "Image Path:" << imgPath;

}



void MainWindow::on_button7Hud_clicked()
{
    std::cout << "Hello" << std::endl;
    setHud("FlawHUD", "FlawHUD Custom hud", "CriticalFlaw", "https://github.com/CriticalFlaw/flawhud/releases/download/2023.1013/flawhud.zip", "flawhud");
    std::vector<QString> imgPaths = createImagesVector("FlawHUD");
    setImages(ui->label_2, imgPaths[0]);
    currentHud->setImages(imgPaths);
}
void MainWindow::on_buttonBudHUD_clicked()
{
    std::cout << "world" << std::endl;

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
    if (index > 0) {
        index--;
    } else {
        index = paths.size() - 1;  // Set index to the last element
    }

    currentHud->setImageNumber(index);

    if (index >= 0 && index < paths.size()) {
        setImages(ui->label_2, paths[index]);
    } else {
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

    // Loop around if index is at the end
    if (index < paths.size() - 1) {
        index++;
    } else {
        index = 0;  // Set index to the first element
    }

    currentHud->setImageNumber(index);

    if (index >= 0 && index < paths.size()) {
        setImages(ui->label_2, paths[index]);
    } else {
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
    getTf2Path.setStyleSheet("* { }");
    if (getTf2Path.exec() == QDialog::Accepted) {
        currentPath = getTf2Path.textValue();
        writePathTxt();

    }
}

void MainWindow::writePathTxt()
{

    QString filePath = "path.txt";  // Adjust the filename as needed
    QFile pathFile(filePath);

    if (pathFile.exists()) {
        // The file exists, open it in write mode to update the content
        if (pathFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&pathFile);
            out << currentPath;
            pathFile.close();

            qDebug() << "TF2 path updated in file: " << currentPath;
        } else {
            qDebug() << "Error: Could not update TF2 path in file.";
        }
    } else {
        // The file doesn't exist, create a new file and save currentPath into it
        if (pathFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&pathFile);
            out << currentPath;
            pathFile.close();

            qDebug() << "TF2 path saved to new file: " << currentPath;
        } else {
            qDebug() << "Error: Could not save TF2 path to file.";
        }
    }
}

