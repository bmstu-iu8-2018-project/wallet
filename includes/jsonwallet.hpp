#pragma once
#ifndef JSONWALLET_HPP
#define JSONWALLET_HPP

#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>


class JsonWallet
{
 public:
    JsonWallet() = default;

    static QString get_name(const QString& fileName);
    static QString get_address(const QString& fileName);
    static QString get_public_key(const QString& fileName);

    static QString get_json(const QString& fileName);
    static void record_to_json(const QVariantMap& map, const QString& fileName);

 private:
    QString get_information(const QString& fileName, const QString& info);

    QJsonDocument load_json(const QString& fileName);

    void save_json(const QJsonDocument& document, const QString& fileName);
};

#endif // JSONWALLET_HPP
