#ifndef CAMERA_H
#define CAMERA_H

#include <QString>
#include <QUrl>

class Camera
{
public:
    Camera();

    Camera(int id, QString name, QString url, QString ftpUser, int ftpPort, QString password, bool isPasswordSaved);

    Camera(QString name, QString url, QString ftpUser, int ftpPort, QString password, bool isPasswordSaved);

    int id;

    QString name;

    QString url;

    QString ftpUser;

    int ftpPort;

    QString password;

    bool isPasswordSaved;
};

#endif // CAMERA_H
