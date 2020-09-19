#include "pathmatcher.h"
#include "pathmatchertest.h"
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QTest>

PathMatcherTest::PathMatcherTest(QObject *parent) : QObject(parent) {}

void PathMatcherTest::regexMatching() {
  QRegularExpression re{"{(\\w+)}"};
  QString path = "/part1/{first_id}/part2/{second_id}";

  auto m = re.globalMatch(path);
  auto match = m.next();
  QCOMPARE(match.hasMatch(), true);
  QCOMPARE(match.captured(1), QString{"first_id"});
  match = m.next();
  QCOMPARE(match.captured(1), QString{"second_id"});
}

void PathMatcherTest::regex2() {
  QRegularExpression re{"/part1/(?<first_id>\\w+)/part2/(?<second_id>\\w+)"};
  QString path = "/part1/pepe/part2/lola";

  auto m = re.match(path);
  QCOMPARE(m.captured("first_id"), "pepe");
  QCOMPARE(m.captured("second_id"), "lola");
}

void PathMatcherTest::matcherStringTest() {
  PathMatcher m{"/part1/{first_id}/part2/{second_id}"};
  QCOMPARE(m.getMatcherString(),
           QString("/part1/(?<first_id>\\w+)/part2/(?<second_id>\\w+)"));
}

void PathMatcherTest::matchTest() {
  PathMatcher m{"/part1/{first_id}/part2/{second_id}"};
  auto res = m.match("/part1/pepe/part2/lola");
  QCOMPARE(res["first_id"], "pepe");
  QCOMPARE(res["second_id"], "lola");
}
QTEST_MAIN(PathMatcherTest)