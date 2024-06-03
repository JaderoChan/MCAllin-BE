#pragma once

#include <QWidget>
#include "ui_about.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AboutClass; };
QT_END_NAMESPACE

class About : public QWidget
{
    Q_OBJECT

public:
    About(QWidget *parent = nullptr) :
        QWidget(parent),
        ui(new Ui::AboutClass())
    {
        ui->setupUi(this);

        ui->donateWgt->hide();
        ui->creditWgt->hide();
        ui->aboutWgt->hide();

        connect(ui->returnBtn, &QPushButton::clicked, this, [&] () {
            close();
        });
    }
    ~About() {
        delete ui;
    }

    void retranslate() {
        ui->retranslateUi(this);
    }

signals:
    void closed();

protected:
    void closeEvent(QCloseEvent *event) override {
        emit closed();
    }

private:
    Ui::AboutClass *ui;
};
