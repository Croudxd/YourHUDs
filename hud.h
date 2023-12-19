#ifndef HUD_H
#define HUD_H

#include <QObject>
#include <QString>
#include <vector>

class hud : public QObject {
    Q_OBJECT

private:
    QString name;
    QString description;
    QString creator;
    QString downloadlink;
    std::vector<QString> images;
    int imageNumber=0;
    QString hudFileName;

public:
    //Constructor
    hud(QObject *parent = nullptr, QString name = "", QString description= "", QString creator= "", QString downloadlink= ""); // Constructor declaration
    // Destructor declaration
    ~hud();
    //Setter functions
    void setName(const QString &name);
    void setDescription(const QString &description);
    void setCreator(const QString &creator);
    void setDownloadLink(const QString &downloadlink);
    void setImages(const std::vector<QString> &imagesVector);
    void setImageNumber(const int index);
    void setHudFileName(const QString hudFileName);

    //Getter functions.

    QString getName() const;
    QString getDescription() const;
    QString getCreator() const;
    QString getDownloadLink() const;
    std::vector<QString> getImages() const;
    int getImageNumber() const;
    QString getHudFileName() const;

};

#endif // HUD_H
