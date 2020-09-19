#ifndef CONFIG_H
#define CONFIG_H

#include "Singleton.h"
#include <QJsonDocument>

#include <QtCore/qglobal.h>

class Config : public Singleton<Config> {
public:
  Config();

  const QJsonValue operator[](const QString &key) const;

private:
  QJsonDocument json;
};

#endif // CONFIG_H
