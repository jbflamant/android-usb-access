#include <QGuiApplication>
#include <QQmlApplicationEngine>

#if defined(Q_OS_ANDROID)
#include "android/HardwareUtils_Android.h"
#endif

#include <QDebug>

int main(int argc, char *argv[])
{
    HardwareUtilsAndroid::Instance();

    QGuiApplication app(argc, argv);

    HardwareUtilsAndroid::Instance()->readDomFiles();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    return app.exec();
}

