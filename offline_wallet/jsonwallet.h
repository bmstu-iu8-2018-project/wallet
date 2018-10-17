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

    QJsonDocument loadJson(QString fileName);

    void saveJson(QJsonDocument document, QString fileName);

    static void recordJson(QVariantMap map, QString fileName);
};

#endif // JSONWALLET_H
