#ifndef PATHMATCHERTEST_H
#define PATHMATCHERTEST_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

class PathMatcherTest : public QObject {
  Q_OBJECT
public:
  explicit PathMatcherTest(QObject *parent = nullptr);

signals:
private slots:
  void regexMatching();
  void regex2();
  void matcherStringTest();
  void matchTest();
};

#endif // PATHMATCHERTEST_H
