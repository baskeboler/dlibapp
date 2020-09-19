#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QDebug>
#include <QObject>
#include <QtCore/qglobal.h>
#include <qhttp/qhttpserverrequest.hpp>
#include <qhttp/qhttpserverresponse.hpp>

using namespace qhttp::server;

class ClientHandler : public QObject {
  Q_OBJECT
public:
  ClientHandler(quint64 id, QHttpRequest *req, QHttpResponse *res);
  virtual ~ClientHandler();

protected:
  quint64 iconnectionId;
signals:
};

#endif // CLIENTHANDLER_H
