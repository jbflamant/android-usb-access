
#include "HardwareUtils_Android.h"
#include <QtAndroid>
#include <jni.h>

#include <QDebug>

HardwareUtilsAndroid *HardwareUtilsAndroid::Instance(QObject *parent) {
    static HardwareUtilsAndroid *instance = NULL;

    if(!instance) {
        instance = new HardwareUtilsAndroid(parent);
    }

    return instance;
}

HardwareUtilsAndroid::HardwareUtilsAndroid(QObject *parent):
    QObject(parent)
{
}

HardwareUtilsAndroid::~HardwareUtilsAndroid()
{
}

bool HardwareUtilsAndroid::writeOnUSB(QString filename) {
    m_file.setFileName(filename);
    QAndroidJniObject ACTION_CREATE_DOCUMENT = QAndroidJniObject::fromString("android.intent.action.CREATE_DOCUMENT");
    QAndroidJniObject intent("android/content/Intent");
    if (ACTION_CREATE_DOCUMENT.isValid() && intent.isValid()) {
        intent.callObjectMethod("setAction", "(Ljava/lang/String;)Landroid/content/Intent;", ACTION_CREATE_DOCUMENT.object<jstring>());
        intent.callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;", QAndroidJniObject::fromString("application/*").object<jstring>());
        QtAndroid::startActivity(intent.object<jobject>(), 123, this);
        return true;
    } else {
        return false;
    }
}

void HardwareUtilsAndroid::handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject & data) {
    qDebug() << "Receive activity result";

    jint RESULT_OK = QAndroidJniObject::getStaticField<jint>("android/app/Activity", "RESULT_OK");
    if (receiverRequestCode == 123 && resultCode == RESULT_OK) {
        QString path = data.callObjectMethod("getData", "()Landroid/net/Uri;").callObjectMethod("getPath", "()Ljava/lang/String;").toString();
        qDebug() << "Destination path : " << path;
    } else {
        qDebug() << "Destination path is empty ";
    }
}
