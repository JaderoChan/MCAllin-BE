#include "intro.h"

Intro::Intro(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::IntroClass())
{
    ui->setupUi(this);
    ui->logoLbl->setPixmap(QPixmap(":/image/resource/image/icon.png"));
    ui->versionCodeLbl->setText(tr("Version") + " : " + VERSION_CODE);

    // Connect.
    connect(ui->settingBtn, &QPushButton::clicked, this, [&] () {
        emit jumpToSetting();
    });
    connect(ui->aboutBtn, &QPushButton::clicked, this, [&] () {
        emit jumpToAbout();
    });
    connect(ui->blockImageBtn, &QPushButton::clicked, this, [&] () {
        emit jumpToBlockImage();
    });
    connect(ui->imagePackBtn, &QPushButton::clicked, this, [&] () {
        emit jumpToImagePack();
    });
    connect(ui->videoPackBtn, &QPushButton::clicked, this, [&] () {
        emit jumpToVideoPack();
    });
}

Intro::~Intro()
{
    delete ui;
}

// TODO
void Intro::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton &&
        ui->versionCodeLbl->underMouse())
    {
        ++mEasterEggCurrentCount;
        if (mEasterEggCurrentCount == mEasterEggTiggerCount) {
            mEasterEggCurrentCount = 0;
            qDebug() << "You found a easter egg.";
        }
    } else {
        mEasterEggCurrentCount = 0;
    }
}
