#ifndef HARDWAREUTILS_ANDROID_H
#define HARDWAREUTILS_ANDROID_H

#include <QObject>
#include <QAndroidActivityResultReceiver>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QFile>

class HardwareUtilsAndroid: public QObject, public QAndroidActivityResultReceiver
{
private:
    HardwareUtilsAndroid(QObject *parent = 0);

public:
    virtual ~HardwareUtilsAndroid();
    static HardwareUtilsAndroid* Instance(QObject *parent = 0);

    virtual void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject & data);
    virtual bool writeOnUSB(QString filename);

private:
    QFile m_file;
};

#endif // HARDWAREUTILS_ANDROID_H
