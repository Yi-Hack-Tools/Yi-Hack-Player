#include "ftpdirswidget.h"
#include "ui_ftpdirswidget.h"

FtpDirsWidget::FtpDirsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FtpDirsWidget)
{
    ui->setupUi(this);

    ui->tableView->setModel(&filesModel);

}

FtpDirsWidget::~FtpDirsWidget()
{
    delete ui;
}

void FtpDirsWidget::addItem(QString name){
    QStandardItem *item = new QStandardItem;
    item->setData(name, Qt::EditRole);
    filesModel.setItem(filesModel.rowCount(), 0, item);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

}
