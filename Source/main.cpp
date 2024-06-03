#include <QtWidgets/QApplication>
#include <QTranslator>

#include "stackedwidget.h"
#include <QSettings>

static ConfigData readConfigFromIni(const QString &path) {
    QSettings settings(path, QSettings::IniFormat);
    ConfigData data;
    data.language = settings.value("language").toString().toStdString();
    data.compress = settings.value("compress").toBool();
    data.detachFrame = settings.value("detachFrame").toBool();
    data.exportPath = settings.value("exportPath").toString().toStdString();
    data.filterAttribute = settings.value("filterAttribute").toInt();
    data.filterVersion = Version(settings.value("filterVersion/major").toInt(),
                                 settings.value("filterVersion/minor").toInt(),
                                 settings.value("filterVersion/patch").toInt());
    data.isFunctionPack = settings.value("isFunctionPack").toBool();
    data.maxCommandCount = settings.value("maxCommandCount").toInt();
    data.maxFrameCount = settings.value("maxFrameCount").toInt();
    data.maxSize[0] = settings.value("maxSize/width").toInt();
    data.maxSize[1] = settings.value("maxSize/height").toInt();
    data.plane = static_cast<Plane>(settings.value("plane").toInt());
    data.useNewExecute = settings.value("useNewExecute").toBool();
    Mcpack::PackManifest manifest;
    manifest.name = settings.value("manifest/name").toString().toStdString();
    manifest.description = settings.value("manifest/description").toString().toStdString();
    manifest.formatVersion = settings.value("manifest/formatVersion").toInt();
    manifest.minVersion[0] = settings.value("manifest/minVersion/major").toInt();
    manifest.minVersion[1] = settings.value("manifest/minVersion/minor").toInt();
    manifest.minVersion[2] = settings.value("manifest/minVersion/patch").toInt();
    manifest.packVersion[0] = settings.value("manifest/packVersion/major").toInt();
    manifest.packVersion[1] = settings.value("manifest/packVersion/minor").toInt();
    manifest.packVersion[2] = settings.value("manifest/packVersion/patch").toInt();
    manifest.prefix = settings.value("manifest/prefix").toString().toStdString();
    manifest.type = static_cast<Mcpack::PackManifest::Type>(settings.value("manifest/type").toInt());
    data.manifest = manifest;
    return data;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConfigData data;
    if(Bf::isExistsFile("./config.ini"))
        data = readConfigFromIni("./config.ini");

    QString language = data.language.c_str();

    QTranslator translator;
    if(translator.load(":/languages/resource/languages/lang_" + language + ".qm"))
        a.installTranslator(&translator);

    StackedWidget w(data);

    QObject::connect(&w, &StackedWidget::langChanged, [&] (const QString &lang) {
        a.removeTranslator(&translator);

        if (translator.load(":/languages/resource/languages/lang_" + lang + ".qm")) {
            a.installTranslator(&translator);
            w.retranslate();
        }
    });

    w.show();
    return a.exec();
}
