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
    void setHud(QString name, QString description, QString creator, QString downloadlink, QString hudFileName);
    std::vector<QString> createImagesVector(QString fileName);
    void setImages(QLabel *label, const QString imgPath);
    QString readHudTxt();
    void installFunction();
    void writeHudTxt();
    void writePathTxt();
    QString readPathTxt();
    void uninstallHud(QString hudtxt);
private slots:

    void on_button7Hud_clicked();

    void on_buttonBudHUD_clicked();

    void on_installbutton_clicked();

    void on_leftImageButton_clicked();

    void on_rightImageButton_clicked();

    void on_actionOptions_triggered();

private:
    Ui::MainWindow *ui;
    hud *currentHud;
    QString currentPath;
};
#endif // MAINWINDOW_H
