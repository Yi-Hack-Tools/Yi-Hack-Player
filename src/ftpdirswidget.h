#ifndef FTPDIRSWIDGET_H
#define FTPDIRSWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include "qt5ftp/qftp/qftp.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "ftpclient-cpp/FTP/FTPClient.h"

namespace Ui {
class FtpDirsWidget;
}

class FtpDirsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FtpDirsWidget(QString host, int port, QString username, QString password, QWidget *parent = nullptr);
    ~FtpDirsWidget();

    void addDirItem(QString name);

    void addMediaItem(QString name);

    QString host;
    int port;
    QString username;
    QString password;

private:
    Ui::FtpDirsWidget *ui;

    QStandardItemModel filesModel;

    QStandardItemModel mediaModel;

    QFtp *ftp;

    QFile *file;

    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply;

    embeddedmz::CFTPClient *cftpClient;

private Q_SLOTS:
    void playMedia();

    void listMedia(QModelIndex index);

    void listDirInfo(const QUrlInfo &info);

    void listMediaInfo(const QUrlInfo &info);

    void loginStateChanged(int state);

    void disconnectDirsListing();

    void downloadFile(QString filename);

signals:
    void videoSelected(QString filename);
};

#endif // FTPDIRSWIDGET_H
