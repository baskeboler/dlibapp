#include "imageviewerform.h"
#include "ui_imageviewerform.h"
#include <QGraphicsRectItem>

#include <QPen>

ImageViewerForm::ImageViewerForm(const QImage &img, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ImageViewerForm), img{img}, scene{new QGraphicsScene{}} {
  ui->setupUi(this);

  scene->addPixmap(QPixmap::fromImage(img));

  ui->graphicsView->setScene(scene);
}

ImageViewerForm::ImageViewerForm(const QImage &img, const QVector<QRect> &rects,
                                 QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ImageViewerForm), img{img}, scene{new QGraphicsScene{}} {
  ui->setupUi(this);
  QPen *pen = new QPen{QColor{255, 0, 0}};
  pen->setStyle(Qt::PenStyle::SolidLine);
  pen->setWidth(4);
  //  scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
  //  scene->set
  scene->addPixmap(QPixmap::fromImage(img));
  //  pix->setZValue(1);
  //  pix->setVisible(true);
  for (const auto &r : rects) {
    auto rectItem = scene->addRect(QRectF{r}, *pen);
    //    rectItem->setVisible(true);
    //    rectItem->setZValue(10);
    scene->addItem(rectItem);
    //    rectItem->stack/(pix);
  }
  //    scene->
  scene->update();
  ui->graphicsView->setScene(scene);
  ui->graphicsView->update();
  ui->graphicsView->show();
}

ImageViewerForm::ImageViewerForm(const QImage &img,
                                 const QVector<QPainterPath> &paths,
                                 QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ImageViewerForm), img{img}, scene{new QGraphicsScene{}} {
  ui->setupUi(this);
  QPen *pen = new QPen{QColor{255, 0, 0}};
  pen->setStyle(Qt::PenStyle::SolidLine);
  pen->setWidth(4);
  //  scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
  //  scene->set
  scene->addPixmap(QPixmap::fromImage(img));
  for (const auto &p : paths) {
    scene->addPath(p, *pen);
  }
  //    scene->
  scene->update();
  ui->graphicsView->setScene(scene);
  ui->graphicsView->update();
  ui->graphicsView->show();
}

ImageViewerForm::~ImageViewerForm() { delete ui; }

void ImageViewerForm::changeEvent(QEvent *e) {
  QWidget::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    ui->retranslateUi(this);
    break;
  default:
    break;
  }
}
