#ifndef PATHMATCHER_H
#define PATHMATCHER_H

#include <QtCore/qglobal.h>

#include <QMap>
#include <QRegularExpression>
#include <QString>

class PathMatcher {
public:
  PathMatcher(const QString &path);

  QString getMatcherString() const;

  QMap<QString, QString> match(QString arg) const;

private:
  QString spec;
  QString matcherString;
  QStringList varNames;
};

#endif // PATHMATCHER_H
