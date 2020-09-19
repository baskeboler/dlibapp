#include "server.h"

#include "clienthandler.h"

#include <qhttp/qhttpserver.hpp>
#include <qhttp/qhttpserverrequest.hpp>
#include <qhttp/qhttpserverresponse.hpp>
using namespace qhttp::server;

Server::Server(QObject *parent) : QHttpServer(parent) {
  // dumb (trivial) connection counter
  quint64 iconnectionCounter = 0;

  listen("8080", [&](QHttpRequest *req, QHttpResponse *res) {
    new ClientHandler{iconnectionCounter++, req, res};
  });
}
