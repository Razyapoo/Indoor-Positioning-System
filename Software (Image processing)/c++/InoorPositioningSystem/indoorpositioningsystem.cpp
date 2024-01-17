#include "indoorpositioningsystem.h"
#include "./ui_indoorpositioningsystem.h"

IndoorPositioningSystem::IndoorPositioningSystem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::IndoorPositioningSystem)
{
    player = new QMediaPlayer();
    frameTimer = new QTimer();
    ui->setupUi(this);
    ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    // connect(player, &QMediaPlayer::positionChanged, this, &IndoorPositioningSystem::updateDuration);
    // connect(player, &QMediaPlayer::positionChanged, this, &IndoorPositioningSystem::updatePosition);

    // connect(ui->horizontalSlider_Duration, &QSlider::valueChanged, this, &IndoorPositioningSystem::onSliderMoved);
    connect(frameTimer, &QTimer::timeout, this, &IndoorPositioningSystem::updateFrame);

    // ui->horizontalSlider_Duration->setRange(0, player->duration() / 1000);
}

IndoorPositioningSystem::~IndoorPositioningSystem()
{
    delete ui;
}

// Private
// void IndoorPositioningSystem::setDuration(qint64 duration)
// {
//     if (duration || mDuration) {
//         QTime currentTime((duration / 3600) % 60, (duration / 60) % 60, duration % 60, (duration * 1000) % 1000);
//         QTime totalTime((mDuration / 3600) % 60, (mDuration / 60) % 60, mDuration % 60, (mDuration * 1000) % 1000);
//         QString format = "";
//         if (mDuration > 3600) {
//             format = "hh:mm:ss";
//         } else {
//             format = "mm:ss";
//         }

//         ui->label_Current_Time->setText(currentTime.toString(format));
//         ui->label_Total_Time->setText(totalTime.toString(format));
//     }
// }

void IndoorPositioningSystem::setFrame() {
    QImage qImage = matToQImage(frame);
    QPixmap pixmap = QPixmap::fromImage(qImage);
    ui->label_Video->setPixmap(pixmap.scaled( ui->label_Video->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

QImage IndoorPositioningSystem::matToQImage(const cv::Mat& mat) {
    switch (mat.type()) {
    case CV_8UC4:
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
    case CV_8UC3:
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888).rgbSwapped();
    case CV_8UC1:
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
    default:
        qWarning("Unsupported format in matToQImage conversion");
        break;
    }
    return QImage();
}

// Private Slots
// void IndoorPositioningSystem::updateDuration(qint64 duration)
// {
//     mDuration = duration / 1000;
//     ui->horizontalSlider_Duration->setMaximum(mDuration);

// }

// void IndoorPositioningSystem::updatePosition(qint64 duration)
// {
//     if (!ui->horizontalSlider_Duration->isSliderDown()){
//         ui->horizontalSlider_Duration->setValue(duration / 1000);
//     }

//     setDuration(duration / 1000);
// }

void IndoorPositioningSystem::updateFrame() {
    if (!camera.read(frame)) {
        frameTimer->stop();
        return;
    }

    setFrame();
    int currentFrame = int(camera.get(cv::CAP_PROP_POS_FRAMES));
    ui->horizontalSlider_Duration->setValue(currentFrame);
}

// void IndoorPositioningSystem::onSliderMoved(int position) {
//     camera.set(cv::CAP_PROP_POS_FRAMES, position);
//     ui->label_Current_Time->setText(QString::number(position));

//     // updateFrame();

// }

void IndoorPositioningSystem::on_pushButton_Play_Pause_clicked()
{
    if (isPause) {
        // player->play();
        frameTimer->start();
        ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    } else {
        // player->pause();
        frameTimer->stop();
        ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }

    isPause = !isPause;

}



void IndoorPositioningSystem::on_actionOpen_Video_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Video"), "", tr("Video Files (*.avi)"));

    // video = new QVideoWidget(ui->label_Video);
    // QVBoxLayout* layout = new QVBoxLayout(ui->label_Video);
    // layout->addWidget(video);

    // video->setGeometry(5,5,ui->label_Video->width() - 10, ui->label_Video->height() - 10);

    // player->setSource(QUrl::fromLocalFile(fileName));
    // player->setVideoOutput(video);

    // connect(player, &QMediaPlayer::errorOccurred, [](QMediaPlayer::Error error) {
    //     qDebug() << "Player error:" << error;
    // });

    // video->show();

    camera.open(filename.toStdString());
    if (!camera.isOpened()) {
        throw std::runtime_error("Failed to open video " + filename.toStdString());
    }

    totalFrames = int(camera.get(cv::CAP_PROP_FRAME_COUNT));
    ui->horizontalSlider_Duration->setRange(1, totalFrames);
    frameRate = camera.get(cv::CAP_PROP_FPS);
    frameTimer->setInterval(1000 / frameRate);

}


void IndoorPositioningSystem::on_horizontalSlider_Duration_valueChanged(int position)
{
    camera.set(cv::CAP_PROP_POS_FRAMES, position);
    ui->label_Current_Time->setText(QString::number(position));

    if (!camera.read(frame)) {
        frameTimer->stop();
        return;
    }

    setFrame();
}


// public
