#pragma once

#include <QWidget>
#include <QStackedWidget>

#include "intro.h"
#include "about.h"
#include "setting.h"
#include "blockimage.h"
#include "imagepack.h"
#include "videopack.h"

class StackedWidget  : public QWidget
{
    Q_OBJECT

public:
    StackedWidget(const ConfigData &data, QWidget *parent = nullptr);
    ~StackedWidget();

    void retranslate() {
        mIntro->retranslate();
        mAbout->retranslate();
        mSetting->retranslate();
        mBlockImage->retranslate();
        mImagePack->retranslate();
        mVideoPack->retranslate();
    }

signals:
    void langChanged(const QString &);

public slots:
    void jumpToSetting();
    void jumpToAbout();
    void jumpToBlockImage();
    void jumpToImagePack();
    void jumpToVideoPack();
    void reintro();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void writeConfig(const QString &path);

private:
    QStackedWidget *mCentral;
    Intro *mIntro;
    About *mAbout;
    Setting *mSetting;
    BlockImage *mBlockImage;
    ImagePack *mImagePack;
    VideoPack *mVideoPack;
};
