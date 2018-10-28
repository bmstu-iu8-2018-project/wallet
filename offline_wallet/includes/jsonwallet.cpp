#include "jsonwallet.h"

QJsonDocument JsonWallet::load_json(QString fileName)
{
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(jsonFile.readAll());
}

void JsonWallet::save_json(QJsonDocument document, QString fileName)
{
    QFile jsonFile(fileName);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
}

void JsonWallet::record_to_json(QVariantMap map, QString fileName)
{
    QJsonObject object = QJsonObject::fromVariantMap(map);
    QJsonDocument document;
    document.setObject(object);

    JsonWallet js;
    js.save_json(document, fileName);
}

QString JsonWallet::get_information(QString fileName, QString info)
{
    QJsonDocument document = load_json(fileName);
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
