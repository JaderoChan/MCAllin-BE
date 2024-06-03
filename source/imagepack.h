#pragma once

#include <QWidget>
#include <QPushButton>

#include "setting.h"
#include "mediaload.h"

class ImagePack : public QWidget
{
    Q_OBJECT

public:
    ImagePack(const ConfigData &data, QWidget *parent = nullptr) :
        QWidget(parent),
        mMediaLoad(new MediaLoad(MediaLoad::Image, this)), mSetting(new Setting(data, this))
    {
        // Set the own attributes.
        if (parent != nullptr)
            setMinimumSize(parent->size());

        // Set the own layout.
        setLayout(new QVBoxLayout());
        layout()->setSpacing(0);
        layout()->setContentsMargins(QMargins(0, 0, 0, 0));

        // Init the member widgets.
        mReturnBtn = new QPushButton(tr("Return"), this);
        mGenerateBtn = new QPushButton(tr("Generate"), this);

        // Set the container.
        QWidget *container1 = new QWidget(this);
        container1->setLayout(new QHBoxLayout());
        container1->layout()->setSpacing(0);
        container1->layout()->setContentsMargins(QMargins(0, 0, 0, 0));

        QWidget *container2 = new QWidget(this);
        container2->setMaximumHeight(90);
        container2->setLayout(new QHBoxLayout());
        container2->layout()->setSpacing(20);
        container2->layout()->setContentsMargins(QMargins(40, 0, 40, 0));
        // Add the widgets to container.
        container1->layout()->addWidget(mMediaLoad);
        container2->layout()->addWidget(mReturnBtn);
        container2->layout()->addWidget(mGenerateBtn);

        // Add the widgets to oneself.
        layout()->addWidget(container1);
        layout()->addWidget(mSetting);
        layout()->addWidget(container2);

        // Set the member widgets.
        mMediaLoad->setSize(size().width(), size().width() * 9 / 16);
        mMediaLoad->setDefaultImage(QPixmap(":/image/resource/image/add.png"));
        //mMediaLoad->setStyleSheet("background:gray");
        mSetting->toggleLanguageShow();
        mSetting->toggleVideoPackShow();
        mSetting->toggleReturnShow();
        mReturnBtn->setMinimumSize(100, 50);
        mGenerateBtn->setMinimumSize(100, 50);
        mReturnBtn->setMaximumSize(150, 65);
        mGenerateBtn->setMaximumSize(150, 65);
        mReturnBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        mGenerateBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

        // Connect the signals and slots.
        connect(mReturnBtn, &QPushButton::clicked, this, [&] () {
            emit closed();
        });
        connect(mGenerateBtn, &QPushButton::clicked, this, &ImagePack::generate);
    }
    ~ImagePack() {};

    // Reset the Media Load widget, reset scrollbar position and config data.
    void reset(const ConfigData &data) {
        mMediaLoad->reset();
        mSetting->resetScrollBar();
        mSetting->setConfigData(data);
    }

    // Change the language.
    void retranslate() {
        mSetting->retranslate();
    }

signals:
    void closed();

protected:
    void closeEvent(QCloseEvent *event) override {
        emit closed();
    }

private:
    void generate() {
        auto data = mSetting->getConfigData();
        QString name = QString(data.manifest.name.c_str()).replace("[$ResourceName]",
                                                                   Bf::getFileName(mMediaLoad->getFilePath().toStdString()).c_str());
        QString desc = QString(data.manifest.description.c_str()).replace("[$ResourceName]",
                                                                          Bf::getFileName(mMediaLoad->getFilePath().toStdString()).c_str());
        QString prefix = QString(data.manifest.prefix.c_str()).replace("[$ResourceName]",
                                                                       Bf::getFileName(mMediaLoad->getFilePath().toStdString()).c_str());
        data.manifest.name = name.toStdString();
        data.manifest.description = desc.toStdString();
        data.manifest.prefix = prefix.toStdString();
        if (data.manifest.name.empty())
            data.manifest.name = Bf::getFileName(mMediaLoad->getFilePath().toStdString());
        if (data.manifest.prefix.empty())
            data.manifest.prefix = data.manifest.name;
        BIRaws raws = getBIRawsByDomFile("./resource/data/bedrock/block_info.json");
        BIModis modis = filterBIRaws(raws, data.plane, data.filterAttribute, data.filterVersion);
        std::string inputPath = mMediaLoad->getFilePath().toStdString();
        std::string outputPath = data.exportPath;
        if (data.isFunctionPack) {
            makeImageFunctionPack(inputPath, outputPath, modis, data.manifest, data.plane,
                                  data.maxSize[0], data.maxSize[1], data.maxCommandCount,
                                  data.useNewExecute, data.compress);
        } else {
            makeImageStructurePack(inputPath, outputPath, modis, data.manifest, data.plane,
                                   data.maxSize[0], data.maxSize[1], data.compress);
        }
    }

private:
    Setting *mSetting;
    MediaLoad *mMediaLoad;
    QPushButton *mReturnBtn;
    QPushButton *mGenerateBtn;
};
