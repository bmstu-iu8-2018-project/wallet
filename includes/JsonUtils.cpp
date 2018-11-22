#include "JsonUtils.h"

QJsonDocument ju::load_json(const QString& fileName)
{
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(jsonFile.readAll());
}

void ju::save_json(const QJsonDocument& document, const QString& fileName)
{
    QFile jsonFile(fileName);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
}

void ju::record_to_json(const QVariantMap& map, const QString& fileName)
{
    QJsonObject object = QJsonObject::fromVariantMap(map);
    QJsonDocument document;
    document.setObject(object);

    ju::save_json(document, fileName);
}

QString ju::get_information(const QString& fileName, const QString& info)
{
    QJsonDocument document = load_json(fileName);
    QJsonObject name = document.object();
    QJsonValue value = name.value(QString(info));
    return value.toString();
}

QString ju::get_json(const QString& fileName)
{
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    return jsonFile.readAll();
}

QString ju::get_name(const QString& fileName)
{
    return ju::get_information(fileName, "name");
}

QString ju::get_address(const QString& fileName)
{
    return ju::get_information(fileName, "address");
}

QString ju::get_public_key(const QString& fileName)
{
    return ju::get_information(fileName, "public_key");
}

QString ju::get_balance(const QString& address)
{
    const auto balance_json_data = nu::get_address_balance(address.toStdString());
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
