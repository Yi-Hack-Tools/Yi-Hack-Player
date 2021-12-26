#include "cameradao.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

CameraDAO::CameraDAO()
{
    if(!database.isOpen()){
        database = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
        database.setDatabaseName("cameras.db");
        if(database.open()){
            qDebug()<<"Database open!";
        }else{
            qDebug()<<database.lastError().databaseText();
        }
    }
}

Camera CameraDAO::select(int id){
    QSqlQuery selectQuery(database);
    selectQuery.prepare("SELECT rowid, "
                        "name, "
                        "url, "
                        "ftp_user, "
                        "ftp_port, "
                        "password, "
                        "password_stored "
                        "FROM camera WHERE rowid = :id");
    selectQuery.bindValue(":id", id);
    selectQuery.exec();
    if(selectQuery.next()){
        return Camera(selectQuery.value("rowid").toInt(),
                      selectQuery.value("name").toString(),
                      selectQuery.value("url").toString(),
                      selectQuery.value("ftp_user").toString(),
                      selectQuery.value("ftp_port").toInt(),
                      selectQuery.value("password").toString(),
                      selectQuery.value("password_stored").toBool());
    }else{
        qDebug()<<selectQuery.lastError();
        return Camera();
    }
}

void CameraDAO::insert(Camera camera){
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO camera (name, url, ftp_user, ftp_port, password, password_stored) "
                        "VALUES (:name, :url, :ftpUser, :ftpPort, :password, :passwordStored)");
    insertQuery.bindValue(":name", camera.name);
    insertQuery.bindValue(":url", camera.url);
    insertQuery.bindValue(":ftpUser", camera.ftpUser);
    insertQuery.bindValue(":ftpPort", camera.ftpPort);
    insertQuery.bindValue(":password", camera.password);
    insertQuery.bindValue(":passwordStored", camera.isPasswordSaved);
    insertQuery.exec();
}

void CameraDAO::remove(int id){
    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM camera WHERE rowid = :id");
    removeQuery.bindValue(":id", id);
    removeQuery.exec();
}

void CameraDAO::update(Camera camera){
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE camera SET name = :name, "
                        "url = :url, "
                        "ftp_user = :ftpUser,"
                        "ftp_port = :ftpPort,"
                        "password = :password, "
                        "password_stored = :passwordStored "
                        "WHERE rowid = :id");
    updateQuery.bindValue(":name", camera.name);
    updateQuery.bindValue(":url", camera.url);
    updateQuery.bindValue(":ftpUser", camera.ftpUser);
    updateQuery.bindValue(":ftpPort", camera.ftpPort);
    updateQuery.bindValue(":password", camera.password);
    updateQuery.bindValue(":passwordStored", camera.isPasswordSaved);
    updateQuery.bindValue(":id", camera.id);
    updateQuery.exec();
}

QList<Camera> CameraDAO::list(){
    QList<Camera> camerasList;

    QSqlQuery countQuery;
    countQuery.prepare("SELECT COUNT(*) FROM camera");
    countQuery.exec();
    int cameraCount = 0;

    if(countQuery.next()){
        cameraCount = countQuery.value(0).toInt();
    }

    for(int i = 1; i < cameraCount + 1; i++){
        Camera camera = select(i);
        camerasList.append(camera);
    }

    return camerasList;
}
