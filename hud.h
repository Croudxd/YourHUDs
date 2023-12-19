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
    /**
     * Function takes a QString as param and will set this->hud.name to the param passed.
     * @brief setName
     * @param name
     */
    void setName(const QString &name);
    /**
     * Function takes a QString as param and will set this->hud.description to the param passed.
     * @brief setDescription
     * @param description
     */
    void setDescription(const QString &description);
        /**
         * Function takes a QString as param and will set this->hud.creator to the param passed.
         * @brief setCreator
         * @param creator
         */
    void setCreator(const QString &creator);
    /**
     * Function takes a QString as param and will set this->hud.downloadlink to the param passed.
     * @brief setCreator
     * @param downloadlink
     */
    void setDownloadLink(const QString &downloadlink);
    /**
     * Function takes a vector of QStrings, QStrings are paths to images located in the resource file. Each HUD has its own prefix in /imgs/containing images1-4 as param and will set this->hud.images to the param passed.
     * @brief setCreator
     * @param imagesVector
     */
    void setImages(const std::vector<QString> &imagesVector);
    /**
     * Function takes a int as param and will set this->hud.imageNumber to the param passed.
     * @brief setCreator
     * @param index
     */
    void setImageNumber(const int index);
    /**
     * Function takes a QString as param and will set this->hud.hudFileName to the param passed.
     * @brief setCreator
     * @param hudFileName
     */
    void setHudFileName(const QString hudFileName);

    //Getter functions.
    /**
     * returns QString name of current object.
     * @brief getName
     * @return
     */
    QString getName() const;
    /**
     * returns QString description of current object.
     * @brief getDescription
     * @return
     */
    QString getDescription() const;
    /**
     * returns QString of the creator (HUD developer).
     * @brief getCreator
     * @return
     */
    QString getCreator() const;
    /**
     * returns QString downloadlink (Link to github download).
     * @brief getDownloadLink
     * @return
     */
    QString getDownloadLink() const;
    /**
     * returns QString vector holding image paths.
     * @brief getImages
     * @return
     */
    std::vector<QString> getImages() const;
    /**
     * returns int of imageNumber.
     * @brief getImageNumber
     * @return
     */
    int getImageNumber() const;
    /**
     * returns hudFileName
     * @brief getHudFileName
     * @return
     */
    QString getHudFileName() const;

};

#endif // HUD_H
