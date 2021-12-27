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

    QtAV::AVPlayer *player;
private:
    Ui::MainWindow *ui;
    QtAV::VideoOutput *videoOutput;

    QWidget *outputWidget;


    QStandardItemModel cameraModel;

    CameraDAO *dao;

    QFtp *ftp;

    QMessageBox *ftpMessageBox;

    FtpDirsWidget *ftpDirsWidget;

    QStringList downloadedFiles;

public Q_SLOTS:
    void playVideo(QString filename);

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

    void loginFinished(int code, bool error);

    void closeEvent(QCloseEvent *event) override;

    void screenshot();

    void imageCaptured(QImage image);

    void videoLoaded();
};
#endif // MAINWINDOW_H
