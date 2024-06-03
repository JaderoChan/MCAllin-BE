#pragma once

#include "ui_setting.h"

#include <QWidget>
#include <QScrollBar>
#include <QDir>

#include "allin_api.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SettingClass; };
QT_END_NAMESPACE

class Setting : public QWidget
{
    Q_OBJECT

public:
    // Be used for ban the combox be changed by wheel.
    class WheelEventFilter : public QObject
    {
    public:
        WheelEventFilter(QObject *parent = nullptr) :
            QObject(parent) {};

    protected:
        bool eventFilter(QObject *watched, QEvent *event) override
        {
            if (event->type() == QEvent::Wheel) {
                return true;
            }
            return QObject::eventFilter(watched, event);
        }
    };

public:
    Setting(const ConfigData &data, QWidget *parent = nullptr) :
        QWidget(parent), ui(new Ui::SettingClass()),
        mWheelEventFilter(new WheelEventFilter(this))
    {
        ui->setupUi(this);
        ui->cancelBtn->hide();
        ui->languageCbx->addItem("简体中文", "zh_CHS");
        ui->languageCbx->addItem("English", "en");
        //ui->languageCbx->addItem("繁體中文", "zh_CHT");

        setConfigData(data);

        installFilter(this, mWheelEventFilter);

        // Connect.
        connect(ui->okBtn, &QPushButton::clicked, this, [&] () {
            // TODO
            close();
        });
        connect(ui->cancelBtn, &QPushButton::clicked, this, [&] () {
            // TODO
            close();
        });
        connect(ui->languageCbx, &QComboBox::currentIndexChanged, this, [&] (int index) {
            emit languageChanged(ui->languageCbx->itemData(index).toString());
        });
    }
    ~Setting() {
        delete ui;
    }

    ConfigData getConfigData() {
        updateConfig();
        return mConfigData;
    }

    void setConfigData(const ConfigData &data) {
        mConfigData = data;
        for (int i = 0; i < ui->languageCbx->count(); ++i) {
            if (ui->languageCbx->itemData(i) != mConfigData.language.c_str())
                continue;
            ui->languageCbx->setCurrentIndex(i);
            break;
        }
        ui->exportPathLe->setText(mConfigData.exportPath.c_str());
        ui->exportMethodCbx->setCurrentIndex(mConfigData.isFunctionPack ? 0 : 1);
        ui->maxCommandCountLe->setText(QString::number(mConfigData.maxCommandCount));
        ui->maxFrameCountLe->setText(QString::number(mConfigData.maxFrameCount));
        ui->maxSizeWidthLe->setText(QString::number(mConfigData.maxSize[0]));
        ui->maxSizeHeightLe->setText(QString::number(mConfigData.maxSize[1]));
        ui->whetherCompressPackCbx->setCurrentIndex(mConfigData.compress ? 0 : 1);
        ui->planeCbx->setCurrentIndex(mConfigData.plane);
        ui->videoProcessMethodCbx->setCurrentIndex(mConfigData.detachFrame ? 0 : 1);
        ui->packNameLe->setText(mConfigData.manifest.name.c_str());
        ui->packDescriptionTe->setText(mConfigData.manifest.description.c_str());
        ui->packFormatVersionCbx->setCurrentText(QString::number(mConfigData.manifest.formatVersion));
        ui->packNamespaceLe->setText(mConfigData.manifest.prefix.c_str());
        ui->packMinEngineMajorLe->setText(QString::number(mConfigData.manifest.minVersion[0]));
        ui->packMinEngineMinorLe->setText(QString::number(mConfigData.manifest.minVersion[1]));
        ui->packMinEnginePatchLe->setText(QString::number(mConfigData.manifest.minVersion[2]));
        ui->packVersionMajorLe->setText(QString::number(mConfigData.manifest.packVersion[0]));
        ui->packVersionMinorLe->setText(QString::number(mConfigData.manifest.packVersion[1]));
        ui->packVersionPatchLe->setText(QString::number(mConfigData.manifest.packVersion[2]));
    }

    void updateConfig() {
        mConfigData.language = ui->languageCbx->currentData().toString().toStdString();
        mConfigData.compress =
            ui->whetherCompressPackCbx->currentIndex() == 0 ? true : false;
        mConfigData.detachFrame =
            ui->videoProcessMethodCbx->currentIndex() == 0 ? true : false;
        QString exportPath = ui->exportPathLe->text();
        if (exportPath.isEmpty())
            exportPath = QDir::currentPath();
        else if (exportPath.contains("[$InstalledPath]"))
            exportPath.replace("[$InstalledPath]", QDir::currentPath());
        mConfigData.exportPath = exportPath.toStdString();
        mConfigData.isFunctionPack = ui->exportMethodCbx->currentIndex() == 0 ? true : false;
        mConfigData.filterAttribute = 0;
        mConfigData.filterVersion = Version(0, 0, 0);
        mConfigData.manifest.packVersion = {
            ui->packVersionMajorLe->text().toInt(),
            ui->packVersionMinorLe->text().toInt(),
            ui->packVersionPatchLe->text().toInt() };
        mConfigData.manifest.formatVersion = ui->packFormatVersionCbx->currentText().toInt();
        mConfigData.manifest.minVersion = {
            ui->packMinEngineMajorLe->text().toInt(),
            ui->packMinEngineMinorLe->text().toInt(),
            ui->packMinEnginePatchLe->text().toInt() };
        mConfigData.maxCommandCount = ui->maxCommandCountLe->text().toInt();
        mConfigData.maxFrameCount = ui->maxFrameCountLe->text().toInt();
        mConfigData.maxSize[0] = ui->maxSizeWidthLe->text().toInt();
        mConfigData.maxSize[1] = ui->maxSizeHeightLe->text().toInt();
        mConfigData.plane = static_cast<Plane>(ui->planeCbx->currentIndex());
        mConfigData.useNewExecute = true;
        mConfigData.manifest.name = ui->packNameLe->text().toStdString();
        mConfigData.manifest.description = ui->packDescriptionTe->document()->toPlainText().toStdString();
        mConfigData.manifest.prefix = ui->packNamespaceLe->text().toStdString();
    }

    void showAll() {
        _showAll(this);
    }
    void hideAll() {
        _hideAll(this);
    }
    void toggleLanguageShow() {
        if (ui->languageWgt->isHidden()) {
            ui->languageWgt->show();
        } else {
            ui->languageWgt->hide();
        }
    }
    void togglePackShow() {
        if (ui->packNameWgt->isHidden()) {
            ui->packNameWgt->show();
            ui->packNamespaceWgt->show();
            ui->packDescriptionWgt->show();
            ui->packVersionWgt->show();
            ui->packFormatVersionWgt->show();
            ui->packMinEngineWgt->show();
        } else {
            ui->packNameWgt->hide();
            ui->packNamespaceWgt->hide();
            ui->packDescriptionWgt->hide();
            ui->packVersionWgt->hide();
            ui->packFormatVersionWgt->hide();
            ui->packMinEngineWgt->hide();
        }
    }
    void toggleExportMethodShow() {
        if (ui->exportMethodWgt->isHidden()) {
            ui->exportMethodWgt->show();
        } else {
            ui->exportMethodWgt->hide();
        }
    }
    void toggleExportPathShow() {
        if (ui->exportPathWgt->isHidden()) {
            ui->exportPathWgt->show();
        } else {
            ui->exportPathWgt->hide();
        }
    }
    void togglePlaneShow() {
        if (ui->planeWgt->isHidden()) {
            ui->planeWgt->show();
        } else {
            ui->planeWgt->hide();
        }
    }
    void toggleMaxSizeShow() {
        if (ui->maxSizeWgt->isHidden()) {
            ui->maxSizeWgt->show();
        } else {
            ui->maxSizeWgt->hide();
        }
    }
    void toggleFilterMethodShow() {
        if (ui->filterMethodWgt->isHidden()) {
            ui->filterMethodWgt->show();
        } else {
            ui->filterMethodWgt->hide();
        }
    }
    void toggleFunctionPackShow() {
        if (ui->maxCommandCountWgt->isHidden()) {
            ui->maxCommandCountWgt->show();
        } else {
            ui->maxCommandCountWgt->hide();
        }
    }
    void toggleVideoPackShow() {
        if (ui->maxFrameCountWgt->isHidden()) {
            ui->maxFrameCountWgt->show();
            ui->videoProcessMethodWgt->show();
        } else {
            ui->maxFrameCountWgt->hide();
            ui->videoProcessMethodWgt->hide();
        }
    }
    void toggleReturnShow() {
        if (ui->returnWgt->isHidden()) {
            ui->returnWgt->show();
        } else {
            ui->returnWgt->hide();
        }
    }

    void resetScrollBar() {
        ui->scrollArea->verticalScrollBar()->setValue(0);
    }

    void retranslate() {
        ui->retranslateUi(this);
    }

signals:
    void closed();
    void languageChanged(const QString &);

protected:
    void closeEvent(QCloseEvent *event) override {
        resetScrollBar();
        emit closed();
    }

private:
    void installFilter(QObject *parent, WheelEventFilter *filter) {
        if (parent == nullptr)
            return;
        for (QObject *var : parent->children()) {
            auto box = qobject_cast<QComboBox *>(var);
            if (box != nullptr) {
                box->installEventFilter(filter);
            }
            installFilter(var, filter);
        }
    }
    void _showAll(QObject *parent) {
        if (parent == nullptr)
            return;
        for (auto &var : parent->children()) {
            QWidget *w = qobject_cast<QWidget *>(var);
            if (w != nullptr && w->objectName().contains("Wgt"))
                w->show();
            _showAll(var);
        }
    };
    void _hideAll(QObject *parent) {
        if (parent == nullptr)
            return;
        for (auto &var : parent->children()) {
            QWidget *w = qobject_cast<QWidget *>(var);
            if (w != nullptr && w->objectName().contains("Wgt"))
                w->hide();
            _hideAll(var);
        }
    }

private:
    Ui::SettingClass *ui;
    WheelEventFilter *mWheelEventFilter;
    ConfigData mConfigData;
};
