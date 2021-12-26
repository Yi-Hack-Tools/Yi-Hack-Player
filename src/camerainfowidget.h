#ifndef CAMERAINFOWIDGET_H
#define CAMERAINFOWIDGET_H

#include <QWidget>
#include "cameradao.h"

namespace Ui {
class CameraInfoWidget;
}

class CameraInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CameraInfoWidget(bool welcome = false, QWidget *parent = nullptr);
    CameraInfoWidget(int cameraId, QWidget *parent = nullptr);
    ~CameraInfoWidget();

    enum Mode{
        New,
        Edit,
        };
    Mode mode;

private:
    Ui::CameraInfoWidget *ui;

    int cameraId;

private Q_SLOTS:
    void save();

    void cancel();

    void seePassword();

signals:
    void cameraUpserted();

};

#endif // CAMERAINFOWIDGET_H
