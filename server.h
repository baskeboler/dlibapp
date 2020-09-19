#ifndef SERVER_H
#define SERVER_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>
#include <qhttp/qhttpserver.hpp>

using qhttp::server::QHttpServer;

class Server : public QHttpServer {
  Q_OBJECT
public:
  explicit Server(QObject *parent = nullptr);

signals:
};

#endif // SERVER_H
