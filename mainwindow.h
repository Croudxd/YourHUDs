#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "hud.h"
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    hud *currentHud;
    QString currentPath;
    QString getCurrentPath() const;
    /**
     * Takes a number of parameters, will create a hud.c object and assign it properties given within function.
     * @brief setHud
     * @param name
     * @param description
     * @param creator
     * @param downloadlink
     * @param hudFileName
     */
    void setHud(QString name, QString description, QString creator, QString downloadlink, QString hudFileName);
    /**
     * Takes a filename (Ie budhud), uses a for loop to create paths to append to a vector, which then is returned. Giving back a vector with paths of HUD pictures.
     * @brief createImagesVector
     * @param fileName
     * @return
     */
    std::vector<QString> createImagesVector(QString fileName);
    /**
     * Takes a label (where to display the image), and an imgPath, (Image to display), will then use Q functions to display image within the selected label.
     * @brief setImages
     * @param label
     * @param imgPath
     */
    void setImages(QLabel *label, const QString imgPath);
    /**
     * Will attempt to read a file called hud.txt, if it is not succesful will return empty QString, if it is successfull, it will return the name of the HUD (Name stored in hud.txt)
     * @brief readHudTxt
     * @return
     */
    QString readHudTxt();
    /**
     *  This function uses QNetwork library to install hud->downloadlink.
     * @brief installFunction
     */
    QString installHud();
    /**
     * This function will take the hud->hudFileName, and write it a txt file called hud.txt. This function is only called when a hud is installed to keep
     * track of whether or not a hud is installed into custom.
     * @brief writeHudTxt
     */
    bool writeHudTxt(QString installPath);
    /**
     * This function writes the currentPath QString variable to a txt file, so when user's close the application it will save the locaiton of the custom folder in path.txt.
     * @brief writePathTxt
     */
    bool writePathTxt(QString &installPath);
    /**
     * Will attempt to read path.txt, if it fails returns empty QString, it will not create a new path.txt.
     * @brief readPathTxt
     * @return
     */
    QString readPathTxt();
    /**
     * this function takes a hudtxt read from hud.txt, if a hud is already installed, and user wants to install another it will
     * take the name of the old hud from hud.txt using readHudTxt, and then recursively delete all files in that path.
     * @brief uninstallHud
     * @param hudtxt
     */
    bool uninstallHud(QString hudtxt);
    /**
     * Function uses miniz library: (https://github.com/richgel999/miniz) will extract the hud from the downloaded_file.zip and then delete the zip.
     * @brief extractHud
     * @param installPath
     * @return
     */
    bool extractHud(const QString &zipFilePath);

    //CUSTOM HUD FUNCTIONS.
    /**
     * To call other functions, such as addbutton, copyHud etc when plus button clicked.
     * @brief plus
     */
    void plus();
    /**
     * Function get name of HUD, and then create button and assign name.
     * @brief addButton
     * @return
     */
    bool addButton();

    /**
     *When clicked will change a variable to false, if any buttons are clicked with delete bool on will get deleted.
     * @brief removeButton
     * @return
     */
    bool removeButton();

    /**
     * Will take two paths, and copy hud from one path to the second path/param.
     * @brief copyHud
     * @return
     */
    bool copyHud();

    /**
     * Returns all of customhud to a string to be put into file.
     * @brief toFile
     * @return
     */
    bool toFile();

    /**
     * Will read the HudFile.txt and generate the associated customhud objects, setting name and path.
     * @brief readHudFile
     * @return
     */
    QString readHudFile();

private slots:

    void on_button7Hud_clicked();

    void on_buttonBudHUD_clicked();

    void on_installbutton_clicked();

    void on_leftImageButton_clicked();

    void on_rightImageButton_clicked();
    void on_buttonpeachHUD_clicked();

    void on_buttonzeHUD_clicked();

    void on_oneHUD_clicked();

    void on_buttonHUD_clicked();

    void on_buttonHUD2_clicked();

    void on_buttonHUD3_clicked();

    void on_buttonHUD4_clicked();

    void on_buttonHUD5_clicked();

    void on_buttonHUD6_clicked();

    void on_addButton_clicked();

    void on_removeButton_clicked();

private:
    Ui::MainWindow *ui;
    bool REMOVE = false;


};
#endif // MAINWINDOW_H
