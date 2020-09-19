#include "clienthandler.h"

#include <qhttp/qhttpserverrequest.hpp>
#include <qhttp/qhttpserverresponse.hpp>

#include <QCoreApplication>
#include <QDateTime>

using namespace qhttp::server;

ClientHandler::ClientHandler(quint64 id, QHttpRequest *req, QHttpResponse *res)
    : QObject(req /* as parent*/), iconnectionId(id) {

  qDebug("connection #%llu ...", id);

  // automatically collect http body(data) upto 1KB
  req->collectData();

  // when all the incoming data are gathered, send some response to client.
  req->onEnd([this, req, res]() {
    qDebug("  connection (#%llu): request from %s:%d\n  method: %s url: %s",
           iconnectionId, req->remoteAddress().toUtf8().constData(),
           req->remotePort(), qhttp::Stringify::toString(req->method()),
           qPrintable(req->url().toString()));
    for (auto h = req->headers().keyBegin(); h != req->headers().keyEnd();
         ++h) {
      qDebug() << *h << " -- " << req->headers()[*h];
    }
    if (req->body().size() > 0)
      qDebug("  body (#%llu): %s", iconnectionId, req->body().constData());

    QString message =
        QString("<b>Hello World</b><form method='post' "
                "enctype='multipart/form-data' action='upload'><input "
                "type='file' "
                "name='thefile'><input type='submit'></form>"
                "\n  packet count = %1\n  time = %2\n")
            .arg(iconnectionId)
            .arg(QLocale::c().toString(QDateTime::currentDateTime(),
                                       "yyyy-MM-dd hh:mm:ss"));

    res->setStatusCode(qhttp::ESTATUS_OK);
    res->addHeaderValue("content-length", message.size());
    res->end(message.toUtf8());

    if (req->headers().keyHasValue("command", "quit")) {
      qDebug("\n\na quit has been requested!\n");
      QCoreApplication::quit();
    }
  });
}

ClientHandler::~ClientHandler() {
  qDebug("  ~ClientHandler(#%llu): I've being called automatically!",
         iconnectionId);
}
