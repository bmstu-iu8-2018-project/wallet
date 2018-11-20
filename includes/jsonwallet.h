#ifndef JSONWALLET_H
#define JSONWALLET_H

#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <QMessageBox>
#include "includes/NetworkUtils.h"


class JsonWallet
{
 public:
    JsonWallet() = default;

    static QString get_name(const QString& fileName);
    static QString get_address(const QString& fileName);
    static QString get_public_key(const QString& fileName);
    static QString get_balance(const QString& address);

    static QString get_json(const QString& fileName);
    static void record_to_json(const QVariantMap& map, const QString& fileName);
 private:
    QString get_information(const QString& fileName, const QString& info);

    QJsonDocument load_json(const QString& fileName);

    void save_json(const QJsonDocument& document, const QString& fileName);
};

#endif // JSONWALLET_H
