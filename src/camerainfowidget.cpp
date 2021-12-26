#include "camerainfowidget.h"
#include "ui_camerainfowidget.h"
#include <QMessageBox>

CameraInfoWidget::CameraInfoWidget(bool welcome, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraInfoWidget)
{
    ui->setupUi(this);
    mode = Mode::New;

    if(welcome){
        ui->welcomeLabel->setText(tr("Welcome! Please register a camera."));
    }
}

CameraInfoWidget::CameraInfoWidget(int cameraId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraInfoWidget)
{
    ui->setupUi(this);
    this->cameraId = cameraId;
    mode = Mode::Edit;
    CameraDAO dao;

    Camera camera = dao.select(cameraId);
    ui->nameLineEdit->setText(camera.name);
    ui->rtspUrlLineEdit->setText(camera.url);
    ui->ftpUserLineEdit->setText(camera.ftpUser);
    ui->ftpPortSpinBox->setValue(camera.ftpPort);
    ui->ftpPasswordLineEdit->setText(camera.password);
    ui->savePasswordCheckBox->setChecked(camera.isPasswordSaved);
}


CameraInfoWidget::~CameraInfoWidget()
{
    delete ui;
}

void CameraInfoWidget::save()
{
    CameraDAO dao;

    if(mode == New){
        Camera camera(ui->nameLineEdit->text(),
                      ui->rtspUrlLineEdit->text(),
                      ui->ftpUserLineEdit->text(),
                      ui->ftpPortSpinBox->value(),
                      ui->ftpPasswordLineEdit->text(),
                      ui->savePasswordCheckBox->isChecked());
        dao.insert(camera);
    }else{
        Camera camera(cameraId,
                      ui->nameLineEdit->text(),
                      ui->rtspUrlLineEdit->text(),
                      ui->ftpUserLineEdit->text(),
                      ui->ftpPortSpinBox->value(),
                      ui->ftpPasswordLineEdit->text(),
                      ui->savePasswordCheckBox->isChecked());
        dao.update(camera);
    }
    emit cameraUpserted();
    close();
}

void CameraInfoWidget::cancel()
{
    QMessageBox cancelMessage;
    cancelMessage.setWindowTitle("Cancel");
    cancelMessage.setText("Are you sure?");
    cancelMessage.addButton(tr("Yes"), QMessageBox::AcceptRole);
    cancelMessage.addButton(tr("No"), QMessageBox::RejectRole);

    if(cancelMessage.exec() == QMessageBox::AcceptRole){
        close();
    }

}

void CameraInfoWidget::seePassword(){
    if(ui->ftpPasswordLineEdit->echoMode() == QLineEdit::Normal){
        ui->ftpPasswordLineEdit->setEchoMode(QLineEdit::Password);
    }else{
        ui->ftpPasswordLineEdit->setEchoMode(QLineEdit::Normal);
    }
}

