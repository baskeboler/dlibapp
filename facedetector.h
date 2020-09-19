#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include "Singleton.h"

#include <QImage>
#include <QPixmap>
#include <QRect>
#include <QVector>
#include <QtCore/qglobal.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>

struct FaceAnalysis {
  QRect rect;
  QImage croppedImage;
  QVector<QPoint> parts;
};

class FaceDetector : public Singleton<FaceDetector> {
public:
  FaceDetector();

private:
  dlib::frontal_face_detector detector;
  dlib::shape_predictor pose_model;

public:
  QVector<QRect> faces(const QImage &img);
  QVector<dlib::full_object_detection> fullFaceDetection(const QImage &img);
  QVector<FaceAnalysis> analyseFaces(const QImage &img);

private:
  QVector<QImage> getSubImages(const QImage &img,
                               const QVector<QRect> &rects) const;
  QImage getSubImage(const QImage &img, const QRect &rect) const;
  dlib::array2d<dlib::rgb_pixel> convertImage(const QImage &img) const;
};

#endif // FACEDETECTOR_H
