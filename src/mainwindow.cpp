#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtAVWidgets>
#include "QtAV/AVPlayer.h"
#include <QtAVWidgets>
#include <QLabel>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "camerainfowidget.h"
#include "noteditabledelegate.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFileInfo>
#include "database.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    videoOutput = new QtAV::VideoOutput(this);
    outputWidget = videoOutput->widget();
    ui->horizontalLayout->addWidget(outputWidget);
    player = new QtAV::AVPlayer;
    player->setRenderer(videoOutput);

    QFileInfo checkDb("cameras.db");

    if(!checkDb.exists()){
        Database db;
        db.open();
        db.create();
    }

    dao = new CameraDAO;

    QList<Camera> camerasList = dao->list();

    if(camerasList.length() == 0){
        CameraInfoWidget *cameraWidget = new CameraInfoWidget(true);
        cameraWidget->show();
        connect(cameraWidget, &CameraInfoWidget::cameraUpserted, this, &MainWindow::updateCameraModel);
    }else{
        updateCameraModel();
    }

    NotEditableDelegate *delegate = new NotEditableDelegate;
    ui->tableView->setItemDelegateForColumn(1, delegate);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::playSelected()
{
    if(ui->tableView->selectionModel()->selectedRows().count() == 0){
        return;
    }
    play(ui->tableView->selectionModel()->selectedRows().first());
}

void MainWindow::play(QModelIndex index){
    QLabel *loadingLabel = new QLabel(this);

    loadingLabel->setText(tr("Loading..."));

    loadingLabel->move(outputWidget->geometry().center());
    loadingLabel->setStyleSheet("color: white; font-size: 25px");
    loadingLabel->setGeometry(outputWidget->geometry().center().x(),
                              outputWidget->geometry().center().y(),
                              outputWidget->geometry().width()/2,
                              25);
    loadingLabel->show();

    player->play(index.siblingAtColumn(2).data(Qt::EditRole).toString());
    connect(player, &QtAV::AVPlayer::loaded, loadingLabel, &QLabel::deleteLater);
    connect(player, &QtAV::AVPlayer::loaded, this, &MainWindow::videoLoaded);
}

void MainWindow::updateCameraModel(){
    cameraModel.clear();

    int row = 0;
    int column = 0;
    QList<Camera> camerasList = dao->list();

    for(const Camera &camera: qAsConst(camerasList)){
        QVariantList attributeList;

        attributeList.append(camera.id);
        attributeList.append(camera.name);
        attributeList.append(camera.url);
        attributeList.append(camera.ftpUser);
        attributeList.append(camera.ftpPort);
        attributeList.append(camera.password);
        attributeList.append(camera.isPasswordSaved);

        column = 0;
        for(const QVariant &attribute : attributeList){
            QStandardItem *item = new QStandardItem;
            item->setData(attribute, Qt::EditRole);
            cameraModel.setItem(row, column, item);
            column++;
        }

        row++;
    }

    ui->tableView->setModel(&cameraModel);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(3);
    ui->tableView->hideColumn(4);
    ui->tableView->hideColumn(5);
    ui->tableView->hideColumn(6);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

void MainWindow::editSelected(){

    if(ui->tableView->selectionModel()->selectedRows().count() == 0){
        return;
    }

    CameraInfoWidget *cameraWidget = new CameraInfoWidget(ui->tableView->selectionModel()->selectedRows().first().siblingAtColumn(0).data().toInt());
    cameraWidget->show();
    connect(cameraWidget, &CameraInfoWidget::cameraUpserted, this, &MainWindow::updateCameraModel);

}

void MainWindow::addCamera(){
    CameraInfoWidget *cameraWidget = new CameraInfoWidget();
    cameraWidget->show();
    connect(cameraWidget, &CameraInfoWidget::cameraUpserted, this, &MainWindow::updateCameraModel);

}

void MainWindow::tableViewContextMenu(const QPoint &pos){
    QMenu menu;
    menu.addAction(ui->actionDeleteCamera);
    menu.addAction(ui->actionAddCamera);
    menu.exec(ui->tableView->mapToGlobal(pos));
}

void MainWindow::deleteCamera(){

    QMessageBox deleteMessage;
    deleteMessage.setWindowTitle(tr("Delete Camera"));
    deleteMessage.setText("Are you sure?");
    deleteMessage.addButton(tr("Yes"), QMessageBox::AcceptRole);
    deleteMessage.addButton(tr("No"), QMessageBox::RejectRole);

    if(deleteMessage.exec() == QMessageBox::AcceptRole){
        CameraDAO dao;
        dao.remove(ui->tableView->selectionModel()->selectedRows().first().siblingAtColumn(0).data().toInt());
        updateCameraModel();
    }
}

void MainWindow::openFtp(){
    if(ui->tableView->selectionModel()->selectedRows().count() == 0){
        return;
    }

    QModelIndex selectedIndex = ui->tableView->selectionModel()->selectedRows().first();

    QString urlStr = selectedIndex.siblingAtColumn(2).data().toUrl().host();
    QString ftpUser = selectedIndex.siblingAtColumn(3).data().toString();
    int ftpPort = selectedIndex.siblingAtColumn(4).data().toInt();
    bool isPasswordSaved = selectedIndex.siblingAtColumn(6).data().toBool();
    QString ftpPassword;

    if(isPasswordSaved){
        ftpPassword = selectedIndex.siblingAtColumn(5).data().toString();
    }else{
        ftpPassword = QInputDialog::getText(this,
                                            tr("FTP Password"),
                                            tr("Password for FTP:"),
                                            QLineEdit::Password);
    }

    ftpDirsWidget = new FtpDirsWidget(urlStr, ftpPort, ftpUser, ftpPassword);

    connect(ftpDirsWidget, &FtpDirsWidget::videoSelected, this, &MainWindow::playVideo);

    ftp = new QFtp;
    ftp->connectToHost(urlStr, ftpPort);
    ftp->login(ftpUser, ftpPassword);

    connect(ftp, &QFtp::stateChanged, this, &MainWindow::loginStateChanged);
    connect(ftp, &QFtp::commandFinished, this, &MainWindow::loginFinished);

    ftpMessageBox = new QMessageBox;
    ftpMessageBox->setText(tr("Logging in..."));
    ftpMessageBox->show();
}

void MainWindow::loginStateChanged(int state){
    if(state == 4){
        ftpMessageBox->deleteLater();
        ftpDirsWidget->setWindowTitle(ui->tableView->selectionModel()->selectedRows().first().siblingAtColumn(1).data().toString() + " FTP");
        ftpDirsWidget->show();
    }
    //ftp->close();
}

void MainWindow::loginFinished(int code, bool error){
    if(error){
        ftpMessageBox->setText(tr("Could not login"));
        ftpDirsWidget->deleteLater();
    }
}

void MainWindow::playVideo(QString filename){
    downloadedFiles.append(filename);
#ifdef __linux__
    player->play("/tmp/" + filename);
#elif _WIN32
    player->play("%localappdata%\\Temp\\" + filename);
#endif
}

void MainWindow::closeEvent(QCloseEvent *event){

    for(const QString &filename : qAsConst(downloadedFiles)){
        QFile file(filename);
        file.open(QIODevice::ReadWrite);
        file.remove();
    }
}

void MainWindow::screenshot(){
    connect(player->videoCapture(), &QtAV::VideoCapture::imageCaptured, this, &MainWindow::imageCaptured);
    player->videoCapture()->capture();
}

void MainWindow::imageCaptured(QImage image){
    image.save("Screenshot "
               + ui->tableView->selectionModel()->selectedRows().first().siblingAtColumn(1).data().toString()
               + " "
               + QDateTime::currentDateTime().toString("yyyy-MM-dd HH mm ss") + ".jpg");

}

void MainWindow::videoLoaded(){
    ui->screenshotBtn->setEnabled(true);
}
