#ifndef JSONWALLET_H
#define JSONWALLET_H

#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>


class JsonWallet
{
 public:
    JsonWallet() = default;

    static QString get_name(QString fileName);
    static QString get_address(QString fileName);
    static QString get_public_key(QString fileName);

    static void record_to_json(QVariantMap map, QString fileName);

 private:
    QString get_information(QString fileName, QString info);

    QJsonDocument load_json(QString fileName);

    void save_json(QJsonDocument document, QString fileName);
};

#endif // JSONWALLET_H
