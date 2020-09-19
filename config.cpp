#include "config.h"

#include <QFile>

Config::Config() : Singleton<Config>{} {

  QFile configFile{":/config.json"};
  if (!configFile.open(QFile::ReadOnly)) {
    qWarning("Couldn't open save file.");
    return;
  }

  QByteArray savedData = configFile.readAll();

  json = QJsonDocument::fromJson(savedData);
}

const QJsonValue Config::operator[](const QString &key) const {
  return json[key];
}
