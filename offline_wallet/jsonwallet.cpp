#include "jsonwallet.h"

QJsonDocument JsonWallet::loadJson(QString fileName)
{
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(jsonFile.readAll());
}

void JsonWallet::saveJson(QJsonDocument document, QString fileName)
{
    QFile jsonFile(fileName);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
}

void JsonWallet::recordJson(QVariantMap map, QString fileName)
{
    QJsonObject object = QJsonObject::fromVariantMap(map);
    QJsonDocument document;
    document.setObject(object);

    JsonWallet js;
    js.saveJson(document, fileName);
}

QString JsonWallet::get_information(QString fileName, QString info)
{
    QJsonDocument document = loadJson(fileName);
    QJsonObject name = document.object();
    QJsonValue value = name.value(QString(info));
    return value.toString();
}


QString JsonWallet::get_name(QString fileName)
{
    JsonWallet js;
    return js.get_information(fileName, "name");
}

QString JsonWallet::get_address(QString fileName)
{
    JsonWallet js;
    return js.get_information(fileName, "address");
}

QString JsonWallet::get_public_key(QString fileName)
{
    JsonWallet js;
    return js.get_information(fileName, "public_key");
}
