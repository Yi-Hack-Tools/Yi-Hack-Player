#include "database.h"
#include <QSqlQuery>
#include <QDebug>

Database::Database()
{
}

void Database::open(){
    database = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
    database.setDatabaseName("cameras.db");
    qDebug()<<database.open();
}

void Database::create(){
    QSqlQuery createQuery;;
    createQuery.exec("CREATE TABLE camera ("
                     "name TEXT NOT NULL, "
                     "url TEXT NOT NULL, "
                     "ftp_port INTEGER, "
                     "ftp_user TEXT, "
                     "password TEXT NULL, "
                     "password_stored INTEGER NOT NULL)");
}
