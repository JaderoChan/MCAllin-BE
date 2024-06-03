#pragma once

#include "ui_intro.h"

#include <QtWidgets/QMainWindow>
#include <QMouseEvent>

#define VERSION_CODE "0.1.0"

QT_BEGIN_NAMESPACE
namespace Ui { class IntroClass; };
QT_END_NAMESPACE

class Intro : public QMainWindow
{
    Q_OBJECT

public:
    Intro(QWidget *parent = nullptr);
    ~Intro();

    void retranslate() {
        ui->retranslateUi(this);
    }

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void jumpToSetting();
    void jumpToAbout();
    void jumpToBlockImage();
    void jumpToImagePack();
    void jumpToVideoPack();

private:
    Ui::IntroClass *ui;
    const int mEasterEggTiggerCount = 10;
    int mEasterEggCurrentCount = 0;
};
