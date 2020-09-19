#ifndef IMAGEVIEWERFORM_H
#define IMAGEVIEWERFORM_H

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QWidget>
#endif
#include <QGraphicsScene>
#include <QImage>

namespace Ui {
class ImageViewerForm;
}

class ImageViewerForm : public QWidget {
  Q_OBJECT

public:
  explicit ImageViewerForm(const QImage &img, QWidget *parent = nullptr);
  ImageViewerForm(const QImage &img, const QVector<QRect> &rects = {},
                  QWidget *parent = nullptr);
  ImageViewerForm(const QImage &img, const QVector<QPainterPath> &paths = {},
                  QWidget *parent = nullptr);

  virtual ~ImageViewerForm();

protected:
  void changeEvent(QEvent *e);

private:
  Ui::ImageViewerForm *ui;
  QImage img;
  QGraphicsScene *scene;
};

#endif // IMAGEVIEWERFORM_H
