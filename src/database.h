#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>

class Database
{
public:
    Database();

    void create();

    void open();

private:
    QSqlDatabase database;

};

#endif // DATABASE_H
