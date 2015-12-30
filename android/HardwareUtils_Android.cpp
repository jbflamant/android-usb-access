
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
    m_domDocumentList = new QList<QDomDocument>();
}

HardwareUtilsAndroid::~HardwareUtilsAndroid()
{
}

bool HardwareUtilsAndroid::writeDomFiles(QList<QDomDocument> *domDocumentList, QList<QString> *stringList) {
    m_domDocumentList = domDocumentList;
    m_stringList = stringList;

    QAndroidJniObject ACTION_OPEN_DOCUMENT_TREE = QAndroidJniObject::getStaticObjectField("android/content/Intent", "ACTION_OPEN_DOCUMENT_TREE", "Ljava/lang/String;");
    QAndroidJniObject EXTERNAL_CONTENT_URI = QAndroidJniObject::getStaticObjectField("android/provider/DocumentsContract$Document", "EXTERNAL_CONTENT_URI", "Landroid/net/Uri;");
    QAndroidJniObject intent = QAndroidJniObject("android/content/Intent", "(Ljava/lang/String;Landroid/net/Uri;)V", ACTION_OPEN_DOCUMENT_TREE.object<jstring>(), EXTERNAL_CONTENT_URI.object<jobject>());

    if (ACTION_OPEN_DOCUMENT_TREE.isValid() && intent.isValid())
    {
        QtAndroid::startActivity(intent.object<jobject>(), 40, this);
        qDebug() << "OK";
        return true;
    }
    else
    {
        qDebug() << "ERRO";
        return false;
    }
}

bool HardwareUtilsAndroid::readDomFiles(void){

    QAndroidJniObject ACTION_OPEN_DOCUMENT_TREE = QAndroidJniObject::getStaticObjectField("android/content/Intent", "ACTION_OPEN_DOCUMENT_TREE", "Ljava/lang/String;");
    QAndroidJniObject EXTERNAL_CONTENT_URI = QAndroidJniObject::getStaticObjectField("android/provider/DocumentsContract$Document", "EXTERNAL_CONTENT_URI", "Landroid/net/Uri;");
    QAndroidJniObject intent = QAndroidJniObject("android/content/Intent", "(Ljava/lang/String;Landroid/net/Uri;)V", ACTION_OPEN_DOCUMENT_TREE.object<jstring>(), EXTERNAL_CONTENT_URI.object<jobject>());

    if (ACTION_OPEN_DOCUMENT_TREE.isValid() && intent.isValid())
    {
        QtAndroid::startActivity(intent.object<jobject>(), 58, this);
        qDebug() << "OK";
        return true;
    }
    else
    {
        qDebug() << "ERRO";
        return false;
    }

}

void HardwareUtilsAndroid::handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject & data) {
    qDebug() << "Receive activity result";

    jint RESULT_OK = QAndroidJniObject::getStaticField<jint>("android/app/Activity", "RESULT_OK");
    if (receiverRequestCode == 40 && resultCode == RESULT_OK) {
        QString path = data.callObjectMethod("getData", "()Landroid/net/Uri;").callObjectMethod("getPath", "()Ljava/lang/String;").toString();
        qDebug() << "Destination path : " << path;
        QStringList list = path.split(":");
        QString *finalPath;

        if(list.at(0) == "/tree/primary")
            finalPath = new QString("/sdcard/" + list.at(1));
        else
            finalPath = new QString("/mnt/UsbDriveA/" + list.at(1));

        for (int i = 0; i < m_stringList->size(); i++){
            QString path = *finalPath + "/" + m_stringList->at(i);
            QFile file(path);

            if (file.open(QIODevice::Truncate | QIODevice::WriteOnly)){
                QByteArray result = m_domDocumentList->at(i).toByteArray();
                file.write(result);
                file.close();
            }
            else{
                qDebug("Failed to open file for writing");
                return;
            }
        }
    }
    else if (receiverRequestCode == 58 && resultCode == RESULT_OK){
        QString path = data.callObjectMethod("getData", "()Landroid/net/Uri;").callObjectMethod("getPath", "()Ljava/lang/String;").toString();
        qDebug() << "Destination path : " << path;
        QStringList list = path.split(":");
        QString *finalPath;

        if(list.at(0) == "/tree/primary")
            finalPath = new QString("/sdcard/" + list.at(1));
        else
            finalPath = new QString("/mnt/UsbDriveA/" + list.at(1));

        QDirIterator it(*finalPath, QStringList() << "*.*", QDir::Files);

        if (m_domDocumentList->size() != 0){
            m_domDocumentList->clear();
        }

        while (it.hasNext()){
            it.next();
            QFile file(it.filePath());

            if(!file.open(QFile::ReadOnly)){
                qDebug() << "Error: Cannot read file " << qPrintable(it.filePath())
                         << ": " << qPrintable(file.errorString());
                continue;
            }

            QString errorStr;
            int errorLine;
            int errorColumn;

            QDomDocument doc;

            if (!doc.setContent(&file, false, &errorStr, &errorLine,
                                &errorColumn)){
                qDebug() << "Error: Parse error at line " << errorLine << ", "
                         << "column " << errorColumn << ": "
                         << qPrintable(errorStr);
                continue;
            }
            m_domDocumentList->push_back(doc);
        }
    }
}
