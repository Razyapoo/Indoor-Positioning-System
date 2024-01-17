#ifndef INDOORPOSITIONINGSYSTEM_H
#define INDOORPOSITIONINGSYSTEM_H

#include <QMainWindow>
#include <QtMultimedia/QtMultimedia>
#include <QtWidgets>
#include <QtMultimediaWidgets/QtMultimediaWidgets>
#include <QtCore>
#include <QtGui>
#include <QTimer>
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class IndoorPositioningSystem;
}
QT_END_NAMESPACE

class IndoorPositioningSystem : public QMainWindow
{
    Q_OBJECT

public:
    IndoorPositioningSystem(QWidget *parent = nullptr);
    ~IndoorPositioningSystem();

    void loadVideo(const std::string& filename);

private slots:
    // void updateDuration(qint64 duration);
    // void updatePosition(qint64 duration);
    void updateFrame();

    void on_pushButton_Play_Pause_clicked();
    void on_actionOpen_Video_triggered();
    void on_horizontalSlider_Duration_valueChanged(int position);

    // void onSliderMoved(int position);

private:
    Ui::IndoorPositioningSystem *ui;
    QMediaPlayer *player;
    QTimer *frameTimer;
    QVideoWidget *video;

    cv::VideoCapture camera;
    cv::Mat frame;
    int totalFrames;
    double frameRate;
    qint64 mDuration;
    bool isPause = false;

    // void setDuration(qint64 duration);
    void setFrame();
    QImage matToQImage(const cv::Mat& mat);

};
#endif // INDOORPOSITIONINGSYSTEM_H
