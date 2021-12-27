#include "ftpdirswidget.h"
#include "ui_ftpdirswidget.h"
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

FtpDirsWidget::FtpDirsWidget(QString host, int port, QString username, QString password, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FtpDirsWidget)
{
    ui->setupUi(this);

    this->host = host;
    this->port = port;
    this->username = username;
    this->password = password;

    ui->dirTableView->setModel(&filesModel);
    ui->mediaTableView->setModel(&mediaModel);

    ftp = new QFtp(this);

    ftp->connectToHost(host, port);
    ftp->login(username, password);

    connect(ftp, &QFtp::stateChanged, this, &FtpDirsWidget::loginStateChanged);
    cftpClient = new embeddedmz::CFTPClient();
}

FtpDirsWidget::~FtpDirsWidget()
{
    delete ui;
    delete cftpClient;
}

void FtpDirsWidget::addDirItem(QString name){
    QStandardItem *item = new QStandardItem;
    item->setData(name, Qt::EditRole);
    filesModel.setItem(filesModel.rowCount(), 0, item);
    ui->dirTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
}

void FtpDirsWidget::addMediaItem(QString name){
    QStandardItem *item = new QStandardItem;
    item->setData(name, Qt::EditRole);
    mediaModel.setItem(mediaModel.rowCount(), 0, item);
    ui->mediaTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    mediaModel.sort(0);
}

void FtpDirsWidget::playMedia(){

    if(!ui->mediaTableView->currentIndex().isValid() || !ui->dirTableView->currentIndex().isValid()){
        return;
    }

    downloadFile("/tmp/sd/record/"
                 + ui->dirTableView->selectionModel()->selectedRows().first().siblingAtColumn(0).data().toString()
                 + "/"
                 + ui->mediaTableView->selectionModel()->selectedRows().first().siblingAtColumn(0).data().toString());

    emit videoSelected(ui->mediaTableView->selectionModel()->selectedRows().first().siblingAtColumn(0).data().toString());
}

void FtpDirsWidget::listMedia(QModelIndex index){
    ftp->list("/tmp/sd/record/" + index.siblingAtColumn(0).data(Qt::EditRole).toString() + "/");
    connect(ftp, &QFtp::listInfo, this, &FtpDirsWidget::listMediaInfo);
}

void FtpDirsWidget::listDirInfo(const QUrlInfo &info){
    addDirItem(info.name());
}

void FtpDirsWidget::listMediaInfo(const QUrlInfo &info){
    addMediaItem(info.name());
}

void FtpDirsWidget::loginStateChanged(int state){
    if (state == 4){
        ftp->list("/tmp/sd/record/");
        connect(ftp, &QFtp::listInfo, this, &FtpDirsWidget::listDirInfo);
    }
    connect(ftp, &QFtp::done, this, &FtpDirsWidget::disconnectDirsListing);
}

void FtpDirsWidget::disconnectDirsListing(){
    disconnect(ftp, &QFtp::listInfo, this, &FtpDirsWidget::listDirInfo);
}

void FtpDirsWidget::downloadFile(QString filename){

    cftpClient->InitSession(host.toStdString(), port, username.toStdString(), password.toStdString());
#ifdef __linux__
    cftpClient->DownloadWildcard(std::string("/tmp/"), filename.toStdString());
#elif _WIN32
    cftpClient->DownloadWildcard("%localappdata%\\Temp", filename.toStdString());
#endif
}
