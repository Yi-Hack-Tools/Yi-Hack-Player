#ifndef FTPDIRSWIDGET_H
#define FTPDIRSWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class FtpDirsWidget;
}

class FtpDirsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FtpDirsWidget(QWidget *parent = nullptr);
    ~FtpDirsWidget();

    void addItem(QString name);

private:
    Ui::FtpDirsWidget *ui;

    QStandardItemModel filesModel;
};

#endif // FTPDIRSWIDGET_H
