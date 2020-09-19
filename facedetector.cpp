#include "facedetector.h"
#include "config.h"
#include <QRect>
#include <QVector>
#include <dlib/image_io.h>
#include <vector>

#include <QElapsedTimer>
#ifdef QT_DEBUG
#include <QDebug>
#include <QResource>
#endif
#include <QJsonValue>

using dlib::get_frontal_face_detector;

FaceDetector::FaceDetector()
    : Singleton<FaceDetector>{}, detector{get_frontal_face_detector()},
      pose_model{} {
  QElapsedTimer timer;
  timer.start();
  qDebug() << "Initializing Face Detector";
  auto config = Config::getInstance();
  QString path = (*config)["shapePredictorPath"].toString();
  dlib::deserialize(path.toStdString()) >> pose_model;
  qDebug() << "Finished initializing Face Detector in " << timer.elapsed()
           << " ms";
}

QVector<QRect> FaceDetector::faces(const QImage &img) {
  using namespace dlib;
  using std::vector;

  array2d<rgb_pixel> dlibImg{convertImage(img)};

  //  pyramid_up(dlibImg);
  // Now tell the face detector to give us a list of bounding boxes
  // around all the faces it can find in the image.
#ifdef QT_DEBUG
  QElapsedTimer timer;
  qDebug() << "detecting faces";
  timer.start();
#endif
  vector<rectangle> dets = detector(dlibImg);
#ifdef QT_DEBUG
  qDebug() << "done detecting faces in " << timer.elapsed() << " ms";
  timer.restart();
#endif
  dlibImg.clear();
  QVector<QRect> ret;
  auto toRect = [](const dlib::rectangle &r) {
    return QRect{static_cast<int>(r.left()), static_cast<int>(r.top()),
                 static_cast<int>(r.width()), static_cast<int>(r.height())};
  };

  std::transform(dets.begin(), dets.end(), ret.begin(), toRect);

  return ret;
}

QVector<dlib::full_object_detection>
FaceDetector::fullFaceDetection(const QImage &img) {
  using namespace dlib;
  using std::vector;

  array2d<rgb_pixel> dlibImg{convertImage(img)};
  vector<rectangle> dets = detector(convertImage(img));
  QVector<full_object_detection> res;

  for (auto &r : dets) {
    auto fod = pose_model(dlibImg, r);
    res.push_back(std::move(fod));
  }
  return res;
}

QVector<FaceAnalysis> FaceDetector::analyseFaces(const QImage &img) {
  return {};
}

QVector<QImage> FaceDetector::getSubImages(const QImage &img,
                                           const QVector<QRect> &rects) const {
  QPixmap pmap = QPixmap::fromImage(img);
  QVector<QImage> res;

  for (const auto &r : rects) {
    QImage subimage = pmap.copy(r).toImage();
    res.push_back(std::move(subimage));
  }
  return res;
}

QImage FaceDetector::getSubImage(const QImage &img, const QRect &rect) const {
  return QPixmap::fromImage(img).copy(rect).toImage();
}

dlib::array2d<dlib::rgb_pixel>
FaceDetector::convertImage(const QImage &img) const {
  using namespace dlib;
  array2d<rgb_pixel> dlibImg{img.height(), img.width()};
#ifdef QT_DEBUG
  QElapsedTimer timer;
  timer.start();
  qDebug() << "starting image qt -> dlib copy";
#endif
  for (int i = 0; i < img.width(); ++i)
    for (int j = 0; j < img.height(); ++j) {
      QRgb pix = img.pixel(i, j);
      dlibImg[j][i] =
          rgb_pixel{(unsigned char)qRed(pix), (unsigned char)qGreen(pix),
                    (unsigned char)qBlue(pix)}; // img.pixel(i, j);
    }
#ifdef QT_DEBUG
  qDebug() << "done copying image in " << timer.elapsed() << " ms";
#endif

  return dlibImg;
}
