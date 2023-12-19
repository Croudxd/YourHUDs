#include "hud.h"
#include <QImage>

hud::hud(QObject *parent, QString name, QString description, QString creator, QString downloadlink) : QObject(parent), name(name), description(description), creator(creator), downloadlink(downloadlink) {
    // Constructor implementation here
}

hud::~hud() {
    // Destructor implementation here (if needed)
}

//Setters
void hud::setName(const QString &name)
{
    this->name = name;
}
void hud::setDescription(const QString &description)
{
    this->description = description;
}
void hud::setCreator(const QString &creator)
{
    this->creator = creator;
}
void hud::setDownloadLink(const QString &downloadlink)
{
    this->downloadlink = downloadlink;
}
void hud::setImages(const std::vector<QString> &imagesVector){
    this->images = imagesVector;
}
void hud::setImageNumber(const int index)
{
    this->imageNumber = index;
}
void hud::setHudFileName(const QString hudFileName)
{
    this->hudFileName = hudFileName;
}

//Getters
QString hud::getName() const
{
    return name;
}
QString hud::getDescription() const
{
    return description;
}
QString hud::getCreator() const
{
    return creator;
}
QString hud::getDownloadLink() const
{
    return downloadlink;
}
std::vector<QString> hud::getImages() const
{
    return images;
}
int hud::getImageNumber() const
{
    return imageNumber;
}
QString hud::getHudFileName() const
{
    return hudFileName;
}
