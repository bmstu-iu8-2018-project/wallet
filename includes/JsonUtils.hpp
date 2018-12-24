#ifndef JSONUTILS_HPP
#define JSONUTILS_HPP

#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QPair>
#include <QVariant>
#include <QVector>
#include <includes/NetworkUtils.hpp>

namespace ju {
QString get_name(const QString& fileName, const QString& key);

QString get_address(const QString& fileName, const QString& key);

QString get_public_key(const QString& fileName, const QString& key);

QString get_balance(const QString& address);

QString get_json(const QString& fileName);

void record_to_json(const QVariantMap& map,
                    const QString& fileName,
                    const QString& key);

QString get_information(const QString& fileName,
                        const QString& info,
                        const QString& key);

QJsonDocument load_json(const QString& fileName, const QString& key);

void save_public_data(const QVariantMap& map, const QString& fileName);

void save_json(const QJsonDocument& document,
               const QString& fileName,
               const QString& key);

QPair<QStringList, QVector<QPair<int, QString>>> get_unload_inputs(
    const QString& address);

}  // namespace ju

#endif  // JSONUTILS_HPP
