#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "facedetector.h"
#include "imageviewerform.h"

#include <QCameraInfo>
#include <QDialog>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMediaMetaData>
#include <QPaintEngine>
#include <QPainterPath>
#include <QString>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  setCamera(QCameraInfo::defaultCamera());
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setCamera(const QCameraInfo &info) {
  cam.reset(new QCamera{info});

  connect(cam.get(), &QCamera::stateChanged, this,
          &MainWindow::updateCameraState);
  connect(cam.get(), QOverload<QCamera::Error>::of(&QCamera::error), this,
          &MainWindow::displayCameraError);

  recorder.reset(new QMediaRecorder{cam.get()});
  connect(recorder.get(), &QMediaRecorder::stateChanged, this,
          &MainWindow::updateRecorderState);

  imageCapture.reset(new QCameraImageCapture{cam.get()});
  imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
  connect(recorder.get(), &QMediaRecorder::durationChanged, this,
          &MainWindow::updateRecordTime);
  connect(recorder.get(),
          QOverload<QMediaRecorder::Error>::of(&QMediaRecorder::error), this,
          &MainWindow::displayRecorderError);

  recorder->setMetaData(QMediaMetaData::Title,
                        QVariant{QLatin1String{"Test Title"}});

  connect(imageCapture.get(), &QCameraImageCapture::readyForCaptureChanged,
          this, &MainWindow::onReadyForCaptureChanged);
  connect(imageCapture.get(), &QCameraImageCapture::imageCaptured, this,
          &MainWindow::onImageCaptured);
  connect(imageCapture.get(), &QCameraImageCapture::imageAvailable, this,
          &MainWindow::onImageAvailable);
  connect(imageCapture.get(), &QCameraImageCapture::imageSaved, this,
          &MainWindow::onImageSaved);
  connect(imageCapture.get(),
          QOverload<int, QCameraImageCapture::Error, const QString &>::of(
              &QCameraImageCapture::error),
          this, &MainWindow::onReadyForCaptureChanged);

  cam->setViewfinder(ui->viewFinder);
  cam->setCaptureMode(QCamera::CaptureStillImage);
  cam->start();
}

void MainWindow::showImage(const QImage &img, const QVector<QRect> &rects) {
  QDialog *dialog = new QDialog{};
  auto layout = new QVBoxLayout{};
  QWidget *widget = static_cast<QWidget *>(new ImageViewerForm{img, rects});
  layout->addWidget(widget);
  dialog->setLayout(layout);

  dialog->setModal(true);
  dialog->show();
}

void MainWindow::showImage(const QImage &img,
                           const QVector<QPainterPath> &paths) {
  QDialog *dialog = new QDialog{};
  auto layout = new QVBoxLayout{};
  QWidget *widget = static_cast<QWidget *>(new ImageViewerForm{img, paths});
  layout->addWidget(widget);
  dialog->setLayout(layout);

  dialog->setModal(true);
  dialog->show();
}

void MainWindow::updateCameraState(QCamera::State state) {
  Q_UNUSED(state);
  std::cout << "cam state changed" << std::endl;
}

void MainWindow::displayCameraError(QCamera::Error error) {
  Q_UNUSED(error);
  std::cout << cam->errorString().toStdString() << std::endl;
}

void MainWindow::updateRecorderState(QMediaRecorder::State state) {
  Q_UNUSED(state);
}

void MainWindow::updateRecordTime(qint64 duration) { Q_UNUSED(duration); }

void MainWindow::displayRecorderError(QMediaRecorder::Error error) {
  Q_UNUSED(error);
  std::cout << recorder->errorString().toStdString() << std::endl;
}

void MainWindow::onReadyForCaptureChanged(bool ready) {
  //  if (ready) {
  //    imageCapture->capture();
  //  }
  Q_UNUSED(ready);
}

void MainWindow::onImageCaptured(int id, const QImage &preview) {
  std::cout << "captured!" << std::endl;
  qInfo() << "starting thread " << QThread::currentThread();
  auto fd = FaceDetector::getInstance();

  auto rs = fd->fullFaceDetection(preview);
  QFont font{};
  font.setBold(false);
  font.setPixelSize(4);
  QVector<QPainterPath> paths;
  for (auto f : rs) {
    int nParts = f.num_parts();
    auto &startingPoint = f.part(0);
    QPainterPath path{QPointF{static_cast<qreal>(startingPoint.x()),
                              static_cast<qreal>(startingPoint.y())}};

    path.addRoundedRect(0, 0, 1, 1, 1, 1);
    for (int i = 1; i < nParts; ++i) {
      auto p = f.part(i);
      path.moveTo(p.x(), p.y());
      //      path.addEllipse(p.x(), p.y(), 2, 2);

      path.addText(p.x(), p.y(), font, QString("%1").arg(i));
    }
    paths.push_back(path);
  }

  showImage(preview, paths);
}

void MainWindow::onImageCaptureError(int id, QCameraImageCapture::Error error,
                                     const QString &errorString) {
  Q_UNUSED(id);
  Q_UNUSED(error);
  Q_UNUSED(errorString);
}

void MainWindow::onImageSaved(int id, const QString &fileName) {
  Q_UNUSED(id);
  Q_UNUSED(fileName);
}

void MainWindow::onImageAvailable(int id, const QVideoFrame &frame) {
  Q_UNUSED(id);
  Q_UNUSED(frame);
  std::cout << "image available!" << std::endl;
}

void MainWindow::on_btnCapture_clicked() { imageCapture->capture(); }

void MainWindow::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  if (rects.isEmpty())
    return;
  QPainter painter{this};
  painter.setBrush(QColor{255, 0, 0});

  painter.drawRects(rects);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {

  switch (event->key()) {
  case Qt::Key_Escape:
    QApplication::exit();
    break;
  default:
    break;
  }
}

void MainWindow::on_actionOpen_image_triggered() {
  QFileDialog dialog{this};
  QString fname = dialog.getOpenFileName();
  qDebug() << fname;
  QImage im{fname};

  onImageCaptured(-1, im);
}
