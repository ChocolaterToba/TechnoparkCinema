
#include "authdialog.h"

AuthDialog::AuthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog)
{
    connect(ui->pushButton_reject, &QPushButton::clicked,
            this, &AuthDialog::reject);
    connect(ui->pushButton_connect, &QPushButton::clicked,
            this, &AuthDialog::accept);
}

AuthDialog::~AuthDialog()
{
    delete ui;
}

void AuthDialog::setConnectionData(const AuthDialog::ConnectionData &connectionData)
{
    ui->lineEdit_server->setText(connectionData.server);
    ui->lineEdit_port->setText(QString::number(connectionData.port));
    ui->lineEdit_userName->setText(connectionData.userName);
    ui->comboBox_gender->setCurrentIndex(connectionData.gender);
    ui->comboBox_userColor->setCurrentText(connectionData.userColor);
}

AuthDialog::ConnectionData AuthDialog::connectionData() const
{
    ConnectionData connectionData;
    connectionData.server = ui->lineEdit_server->text();
    connectionData.port = ui->lineEdit_port->text().toInt();
    connectionData.userName = ui->lineEdit_userName->text();

    return connectionData;
}
