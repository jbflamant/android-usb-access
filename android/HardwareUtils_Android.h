#ifndef HARDWAREUTILS_ANDROID_H
#define HARDWAREUTILS_ANDROID_H

#include <QObject>
#include <QAndroidActivityResultReceiver>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QFile>
#include <QDirIterator>
#include <QDomDocument>

class HardwareUtilsAndroid: public QObject, public QAndroidActivityResultReceiver
{
private:
    HardwareUtilsAndroid(QObject *parent = 0);

public:
    virtual ~HardwareUtilsAndroid();
    static HardwareUtilsAndroid* Instance(QObject *parent = 0);

    virtual void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject & data);
    virtual bool writeDomFiles(QList<QDomDocument> *domDocumentList, QList<QString> *stringList);
    virtual bool readDomFiles(void);

private:
    QString m_fileName;
    QList<QDomDocument> *m_domDocumentList;
    QList<QString> *m_stringList;
};

#endif // HARDWAREUTILS_ANDROID_H
