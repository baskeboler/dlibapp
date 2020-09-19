#include "pathmatcher.h"

#include <QRegularExpression>
#include <QString>

PathMatcher::PathMatcher(const QString &path) : spec{path}, varNames{} {

  QRegularExpression re{"{(\\w+)}"};
  QRegularExpressionMatchIterator m = re.globalMatch(spec);

  while (m.hasNext()) {
    auto match = m.next();
    auto v = match.captured(1);
    varNames << v;
  }

  QString newre{path};
  for (auto &v : varNames) {
    auto old = QString{"{%1}"}.arg(v);
    auto replacement = QString{"(?<%1>\\w+)"}.arg(v);
    newre = newre.replace(old, replacement);
  }
  matcherString = std::move(newre);
  //  m.
}

QString PathMatcher::getMatcherString() const { return matcherString; }

QMap<QString, QString> PathMatcher::match(QString arg) const {
  QRegularExpression re{matcherString};
  auto res = re.match(arg);
  QMap<QString, QString> ret;
  for (auto &v : varNames) {
    ret[v] = res.captured(v);
  }
  return ret;
}
