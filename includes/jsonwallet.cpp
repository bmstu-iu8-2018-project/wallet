#include "jsonwallet.h"

QJsonDocument JsonWallet::load_json(const QString& fileName)
{
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(jsonFile.readAll());
}

void JsonWallet::save_json(const QJsonDocument& document, const QString& fileName)
{
    QFile jsonFile(fileName);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
}

void JsonWallet::record_to_json(const QVariantMap& map, const QString& fileName)
{
    QJsonObject object = QJsonObject::fromVariantMap(map);
    QJsonDocument document;
    document.setObject(object);

    JsonWallet js;
    js.save_json(document, fileName);
}

QString JsonWallet::get_information(const QString& fileName, const QString& info)
{
    QJsonDocument document = load_json(fileName);
    QJsonObject name = document.object();
    QJsonValue value = name.value(QString(info));
    return value.toString();
}

QString JsonWallet::get_json(const QString& fileName)
{
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    return jsonFile.readAll();
}

QString JsonWallet::get_name(const QString& fileName)
{
    JsonWallet js;
    return js.get_information(fileName, "name");
}

QString JsonWallet::get_address(const QString& fileName)
{
    JsonWallet js;
    return js.get_information(fileName, "address");
}

QString JsonWallet::get_public_key(const QString& fileName)
{
    JsonWallet js;
    return js.get_information(fileName, "public_key");
}

QString JsonWallet::get_balance(const QString& address)
{
    const auto balance_json_data = get_address_balance(address.toStdString());
    QJsonDocument json = QJsonDocument::fromJson(balance_json_data.c_str());
    QJsonObject all_json = json.object();
    QJsonValue status = all_json.value(QString("status"));
    if (status.toString() == "success")
    {
        QJsonObject data = all_json["data"].toObject();
        QJsonValue balanse = data.value(QString("confirmed_balance"));
        return balanse.toString();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Message");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Check internet connection!");
        msgBox.exec();
        return "No connection";
    }
}
