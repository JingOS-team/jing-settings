/***************************************************************************
 *                                                                         *
 *   Copyright 2011-2015 Sebastian Kügler <sebas@kde.org> 
 *             2021          Wang Rui <wangrui@jingos.com>          
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

// std
#include <iomanip>
#include <iostream>

// own
#include "module.h"
#include "modulesmodel.h"
#include "settingsapp.h"
#include "networkstatus.h"

// Qt
#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>

// Frameworks
#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <KPackage/PackageLoader>
#include <KPluginLoader>
#include <KPluginMetaData>

#include <KPackage/Package>
// #include <KPackage/PackageLoader>
#include <kdeclarative/qmlobjectsharedengine.h>
#include "config.h"
#include <QDBusConnection>
#include <QDBusError>
#include <QDateTime>

#include <KDBusService>
#include <KCrash>
#include <QQmlDebuggingEnabler>

#include <japplicationqt.h>

static constexpr char version[] = "2.0";

int main(int argc, char **argv)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();
    qDebug()<<Q_FUNC_INFO << " loadtime:: main start time:" << startTime;
    // qputenv("QT_SCALE_FACTOR", "2");
    QApplication app(argc, argv);
    JApplicationQt japp;
    japp.enableBackgroud(true);
    // QQmlDebuggingEnabler debugprofile;
    // QQmlDebuggingEnabler::startTcpDebugServer(8003, QQmlDebuggingEnabler::WaitForClient);
    // KLocalizedString::setApplicationDomain("plasma-settings");
    KLocalizedString::setApplicationDomain("settings"); 

    // About data
    KAboutData aboutData("mobile.plasmasettings", i18n("Settings"), version, i18n("Touch-friendly settings application."), KAboutLicense::GPL, i18n("Copyright 2011-2015, Sebastian Kügler"));
    aboutData.addAuthor(i18n("Sebastian Kügler"), i18n("Maintainer"), "sebas@kde.org");
    aboutData.addAuthor(i18n("Marco Martin"), i18n("Maintainer"), "mart@kde.org");
    aboutData.setDesktopFileName("org.kde.mobile.plasmasettings");
    KAboutData::setApplicationData(aboutData);

    QApplication::setWindowIcon(QIcon::fromTheme("preferences-system"));

    QCommandLineParser parser;

    const QCommandLineOption listOption({QStringLiteral("l"), QStringLiteral("list")}, i18n("List available settings modules"));
    const QCommandLineOption formfactorOption(
        {QStringLiteral("x"), QStringLiteral("formfactor")}, i18n("Limit to modules suitable for <formfactor>, e.g. handset, tablet, mediacenter, desktop, test, all (default handset)"), i18n("formfactor"));
    const QCommandLineOption moduleOption(
        {QStringLiteral("m"), QStringLiteral("module")}, 
        i18n("Settings module to open"), i18n("modulename")
        );


    const QCommandLineOption singleModuleOption({QStringLiteral("s"), QStringLiteral("singleModule")}, i18n("Only show a single module, requires --module"));
    const QCommandLineOption fullscreenOption({QStringLiteral("f"), QStringLiteral("fullscreen")}, i18n("Start window fullscreen"));
    const QCommandLineOption layoutOption(QStringLiteral("layout"), i18n("Package to use for the UI (default org.kde.mobile.settings)"), i18n("packagename"));

    parser.addOption(listOption);
    parser.addOption(formfactorOption);
    parser.addOption(moduleOption);
    parser.addOption(singleModuleOption);
    parser.addOption(fullscreenOption);
    parser.addOption(layoutOption);
    aboutData.setupCommandLine(&parser);

    parser.process(app);
    aboutData.processCommandLine(&parser);

     

    if (parser.isSet(listOption)) {
        int nameWidth = 24;
        QSet<QString> seen;
        std::cout << std::setfill('.');

        auto formfactor = parser.value(formfactorOption);

        for (const auto &plugin : KPackage::PackageLoader::self()->listPackages(QString(), "kpackage/kcms/")) {
            if (seen.contains(plugin.pluginId())) {
                continue;
            }
            // Filter out modules that are not explicitly suitable for the "handset" formfactor
            // const QStringList &formFactors = plugin.formFactors();
            if (!formfactor.isEmpty() && !plugin.formFactors().contains(formfactor) && formfactor != QStringLiteral("all")) {
                continue;
            }
            const int len = plugin.pluginId().length();
            if (len > nameWidth) {
                nameWidth = len;
            }

            seen << plugin.pluginId();
            std::cout << plugin.pluginId().toLocal8Bit().data() << ' ' << std::setw(nameWidth - plugin.pluginId().length() + 2) << '.' << ' ' << plugin.description().toLocal8Bit().data() << std::endl;
        }

        for (const auto &plugin : KPluginLoader::findPlugins("kcms")) {
            if (seen.contains(plugin.pluginId())) {
                continue;
            }
            if (!formfactor.isEmpty() && !plugin.formFactors().contains(formfactor) && formfactor != QStringLiteral("all")) {
                continue;
            }
            const int len = plugin.pluginId().length();
            if (len > nameWidth) {
                nameWidth = len;
            }
            std::cout << plugin.pluginId().toLocal8Bit().data() << ' ' << std::setw(nameWidth - plugin.pluginId().length() + 2) << '.' << ' ' << plugin.description().toLocal8Bit().data() << std::endl;
        }

        return 0;
    }

    const QString module = parser.value(moduleOption);
    QString ui = parser.isSet(layoutOption) ? parser.value(layoutOption) : "org.kde.plasma.settings";
    const bool singleModule = parser.isSet(singleModuleOption);

    if (singleModule && module.isEmpty()) {
        parser.showHelp();
        return 0;
    }

    KPackage::Package package = KPackage::PackageLoader::self()->loadPackage("KPackage/GenericQML");
    package.setPath(ui);

    auto *settingsApp = new SettingsApp(parser);
    settingsApp->setStartModule(module);
    settingsApp->setSingleModule(singleModule);

    qmlRegisterType<SettingsConfig>("org.kde.plasma.settings", 0, 1, "SettingsConfig");
    qmlRegisterType<ModulesModel>("org.kde.plasma.settings", 0, 1, "ModulesModel");
    qmlRegisterType<Module>("org.kde.plasma.settings", 0, 1, "Module");
    qmlRegisterType<NetworkStatus>("org.kde.plasma.settings", 0, 1, "NetworkStatus");
    qmlRegisterSingletonInstance<SettingsApp>("org.kde.plasma.settings", 0, 1, "SettingsApp", settingsApp);

    //[liubangguo]app pause signal
    QObject::connect(&japp,&JApplicationQt::background,settingsApp,&SettingsApp::onPaused);
    QObject::connect(&japp,&JApplicationQt::resume,settingsApp,&SettingsApp::onResume);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.rootContext()->setContextProperty("MainStartTime",startTime);

    SettingsConfig settingsConfig;

    settingsConfig.setSelectName("wifi");
    engine.rootContext()->setContextProperty(QStringLiteral("_settingsConfig"), &settingsConfig);
    
    // qputenv("QML_DISABLE_DISK_CACHE", "1");
    engine.load(package.filePath("mainscript"));
    
    //KDBusService service(KDBusService::Unique | KDBusService::StartupOption(KDBusService::Replace));
    //KCrash::setFlags(KCrash::AutoRestart);
    KCrash::setDrKonqiEnabled(false);

    return app.exec();
}
