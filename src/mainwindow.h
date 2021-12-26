#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtAV>
#include <QStandardItemModel>
#include "cameradao.h"
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "qt5ftp/qftp/qftp.h"
#include "qt5ftp/qftp/qurlinfo.h"
#include <QMessageBox>
#include "ftpdirswidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; };
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QtAV::VideoOutput *videoOutput;

    QWidget *outputWidget;

    QtAV::AVPlayer *player;

    QStandardItemModel cameraModel;

    CameraDAO *dao;

    QFtp *ftp;

    QMessageBox *ftpMessageBox;

    FtpDirsWidget *ftpDirsWidget;

private Q_SLOTS:
    void playSelected();

    void play(QModelIndex index);

    void updateCameraModel();

    void editSelected();

    void addCamera();

    void tableViewContextMenu(const QPoint &pos);

    void deleteCamera();

    void openFtp();

    void loginStateChanged(int state);

    void listInfo(const QUrlInfo &info);

    void loginFinished(int code, bool error);

};
#endif // MAINWINDOW_H
