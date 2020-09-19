#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/qglobal.h>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#else
#include <QtGui/QMainWindow>
#endif
#include <QCamera>
#include <QCameraImageCapture>
#include <QMediaRecorder>
#include <dlib/smart_pointers.h>

using dlib::scoped_ptr;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void setCamera(const QCameraInfo &info);
  void showImage(const QImage &img, const QVector<QRect> &rects = {});
  void showImage(const QImage &img, const QVector<QPainterPath> &paths = {});

private:
  Ui::MainWindow *ui;
  scoped_ptr<QCamera> cam;
  scoped_ptr<QCameraImageCapture> imageCapture;
  scoped_ptr<QMediaRecorder> recorder;

  QImageEncoderSettings imageSettings;
  QAudioEncoderSettings audioSettings;
  QVideoEncoderSettings videoSettings;
  QVector<QRect> rects;
  QString videoContainerFormat;

public slots:
  void updateCameraState(QCamera::State state);
  void displayCameraError(QCamera::Error error);
  void updateRecorderState(QMediaRecorder::State state);
  void updateRecordTime(qint64 duration);
  void displayRecorderError(QMediaRecorder::Error error);

  void onReadyForCaptureChanged(bool ready);
  void onImageCaptured(int id, const QImage &preview);
  void onImageCaptureError(int id, QCameraImageCapture::Error error,
                           const QString &errorString);
  void onImageSaved(int id, const QString &fileName);
  void onImageAvailable(int id, const QVideoFrame &frame);
private slots:
  void on_btnCapture_clicked();

  // QWidget interface
  void on_actionOpen_image_triggered();

protected:
  // QWidget interface
protected:
  virtual void paintEvent(QPaintEvent *event) override;

  // QWidget interface
protected:
  virtual void keyPressEvent(QKeyEvent *event) override;
};
#endif // MAINWINDOW_H
