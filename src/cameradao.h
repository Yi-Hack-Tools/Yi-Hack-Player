#ifndef CAMERADAO_H
#define CAMERADAO_H

#include "camera.h"
#include <QSqlDatabase>
#include "database.h"

class CameraDAO
{
public:
    CameraDAO();

    Camera select(int id);

    void insert(Camera camera);

    void remove(int id);

    void update(Camera camera);

    QList<Camera> list();

    QSqlDatabase database;

};

#endif // CAMERADAO_H
