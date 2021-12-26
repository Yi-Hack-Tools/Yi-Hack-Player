#include "camera.h"

Camera::Camera()
{

}

Camera::Camera(int id, QString name, QString url, QString ftpUser, int ftpPort, QString password, bool isPasswordSaved)
{
    this->id = id;
    this->name = name;
    this->url = url;
    this->ftpUser = ftpUser;
    this->ftpPort = ftpPort;
    this->password = password;
    this->isPasswordSaved = isPasswordSaved;
}

Camera::Camera(QString name, QString url, QString ftpUser, int ftpPort, QString password, bool isPasswordSaved)
{
    this->name = name;
    this->url = url;
    this->ftpUser = ftpUser;
    this->ftpPort = ftpPort;
    this->password = password;
    this->isPasswordSaved = isPasswordSaved;
}
