#include "stackedwidget.h"

#include <QLayout>
#include <QSettings>

void StackedWidget::writeConfig(const QString &path) {
    QSettings settings(path, QSettings::IniFormat);
    ConfigData data = mSetting->getConfigData();
    settings.setValue("language", data.language.c_str());
    settings.setValue("compress", data.compress);
    settings.setValue("detachFrame", data.detachFrame);
    settings.setValue("exportPath", data.exportPath.c_str());
    settings.setValue("filterAttribute", data.filterAttribute);
    settings.setValue("filterVersion/major", data.filterVersion.major);
    settings.setValue("filterVersion/minor", data.filterVersion.minor);
    settings.setValue("filterVersion/patch", data.filterVersion.patch);
    settings.setValue("isFunctionPack", data.isFunctionPack);
    settings.setValue("maxCommandCount", data.maxCommandCount);
    settings.setValue("maxFrameCount", data.maxFrameCount);
    settings.setValue("maxSize/width", data.maxSize[0]);
    settings.setValue("maxSize/height", data.maxSize[1]);
    settings.setValue("plane", static_cast<int>(data.plane));
    settings.setValue("useNewExecute", data.useNewExecute);
    settings.setValue("manifest/name", data.manifest.name.c_str());
    settings.setValue("manifest/description", data.manifest.description.c_str());
    settings.setValue("manifest/formatVersion", data.manifest.formatVersion);
    settings.setValue("manifest/minVersion/major", data.manifest.minVersion[0]);
    settings.setValue("manifest/minVersion/minor", data.manifest.minVersion[1]);
    settings.setValue("manifest/minVersion/patch", data.manifest.minVersion[2]);
    settings.setValue("manifest/packVersion/major", data.manifest.packVersion[0]);
    settings.setValue("manifest/packVersion/minor", data.manifest.packVersion[1]);
    settings.setValue("manifest/packVersion/patch", data.manifest.packVersion[2]);
    settings.setValue("manifest/prefix", data.manifest.prefix.c_str());
    settings.setValue("manifest/type", static_cast<int>(data.manifest.type));
}

StackedWidget::StackedWidget(const ConfigData &data, QWidget *parent) :
    QWidget(parent),
    mCentral(new QStackedWidget(this)),
    mSetting(new Setting(data, this)),
    mBlockImage(new BlockImage(mSetting->getConfigData(), this)),
    mImagePack(new ImagePack(mSetting->getConfigData(), this)),
    mVideoPack(new VideoPack(mSetting->getConfigData(), this)),
    mIntro(new Intro(this)), mAbout(new About(this))
{
    // Set the own atrribute.
    setMinimumSize(QSize(900, 800));
    setLayout(new QHBoxLayout());
    setWindowIcon(QIcon(":/image/resource/image/icon.png"));

    // Set the own layout.
    layout()->addWidget(mCentral);

    // Add the sub widgets to the widget.
    mCentral->setLayout(new QHBoxLayout());
    mCentral->addWidget(mIntro);
    mCentral->addWidget(mAbout);
    mCentral->addWidget(mSetting);
    mCentral->addWidget(mBlockImage);
    mCentral->addWidget(mImagePack);
    mCentral->addWidget(mVideoPack);

    // Set the current widget of the StackedWidget.
    mCentral->setCurrentWidget(mIntro);

    // Connect the signals and slots.
    connect(mIntro, &Intro::jumpToAbout, this, &StackedWidget::jumpToAbout);
    connect(mIntro, &Intro::jumpToSetting, this, &StackedWidget::jumpToSetting);
    connect(mIntro, &Intro::jumpToBlockImage, this, &StackedWidget::jumpToBlockImage);
    connect(mIntro, &Intro::jumpToImagePack, this, &StackedWidget::jumpToImagePack);
    connect(mIntro, &Intro::jumpToVideoPack, this, &StackedWidget::jumpToVideoPack);

    connect(mAbout, &About::closed, this, &StackedWidget::reintro);
    connect(mSetting, &Setting::closed, this, &StackedWidget::reintro);
    connect(mBlockImage, &BlockImage::closed, this, &StackedWidget::reintro);
    connect(mImagePack, &ImagePack::closed, this, &StackedWidget::reintro);
    connect(mVideoPack, &VideoPack::closed, this, &StackedWidget::reintro);

    connect(mSetting, &Setting::languageChanged, this, [&] (const QString &lang) {
        emit langChanged(lang);
    });
}

StackedWidget::~StackedWidget()
{}

void StackedWidget::reintro() {
    mCentral->setCurrentWidget(mIntro);
}

void StackedWidget::closeEvent(QCloseEvent *event)
{
    writeConfig("./config.ini");
}

void StackedWidget::jumpToAbout() {
    mCentral->setCurrentWidget(mAbout);
}

void StackedWidget::jumpToSetting() {
    mCentral->setCurrentWidget(mSetting);
}

void StackedWidget::jumpToBlockImage() {
    mBlockImage->reset(mSetting->getConfigData());
    mCentral->setCurrentWidget(mBlockImage);
}

void StackedWidget::jumpToImagePack() {
    mImagePack->reset(mSetting->getConfigData());
    mCentral->setCurrentWidget(mImagePack);
}

void StackedWidget::jumpToVideoPack() {
    mVideoPack->reset(mSetting->getConfigData());
    mCentral->setCurrentWidget(mVideoPack);
}
