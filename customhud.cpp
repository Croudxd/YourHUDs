#include "customhud.h"

customhud::customhud()
{

}
customhud::~customhud(){

}

QString customhud::getName() const
{
    return name;
}
void customhud::setName(QString name)
{
    this->name = name;
}
QString customhud::getPath() const
{
    return path;
}
void customhud::setPath(QString path)
{
    this->path= path;
}


