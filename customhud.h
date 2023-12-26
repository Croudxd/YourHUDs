#ifndef CUSTOMHUD_H
#define CUSTOMHUD_H
#include <QString>



class customhud
{
public:
    customhud();
    ~customhud();
    /**
     * will return name variable from customhud class. (QString)
     * @brief getName
     * @return
     */
    QString getName() const;
    /**
     * Returns path from path variable in customhud (QStirng)
     * @brief getPath
     * @return
     */
    QString getPath() const;

    /**
     * Takes a simple QString, and sets associated hud path as the param.
     * @brief setPath
     * @param path
     */
    void setPath(QString path);
    /**
     * Takes a simple QString, and sets associated hud name  as the param.

     * @brief setName
     * @param name
     */
    void setName(QString name);


private:
    QString name;
    QString path;
};

#endif // CUSTOMHUD_H
