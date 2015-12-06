#ifndef TTS_GUI_H
#define TTS_GUI_H

#include <QMainWindow>

#define WIN32_LEAN_AND_MEAN // Necessary due to problems with Boost
#include "ReadSensors.h"
#include "Magnet.h"
#include "common.h"
#include "Sensor.h"
#include "sensordisplay.h"
#include "videothread.h"

#include <string>
#include <vector>

#include <QVector>
#include <QAudioRecorder>
#include <QAudioProbe>
#include <QaudioBuffer>
#include <QTimer>

#include <QCamera>
#include <QImage>

#include "CImg.h"
using namespace cimg_library;

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

/* **************************************************** *
 *              Variables                               *
 * **************************************************** */
private:
    Ui::MainWindow *ui;
    Magnet magnet;
    QVector<Sensor*> sensors;
    ReadSensors rs;
    VideoThread video;

    // Audio
    QAudioRecorder *audio1;
    QAudioRecorder *audio2;
    QAudioProbe *audioProbe1;
    QAudioProbe *audioProbe2;

    // Utterances
    QList<QString> classUtter;
    QList<QStringList*> utter;
    int numTrials;

    // Output File locations
    QString experiment_root;
    QString experiment_class;
    QString experiment_utter;
    QString experiment_output_path;
    QString emfFile;

    // Display sensor UI
    SensorDisplay *sensorUi;


/* **************************************************** *
 *              Methods                                 *
 * **************************************************** */
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);


private slots:
    void on_configButton_clicked();
    void on_measureEMFButton_clicked();
    void on_startStopTrialButton_toggled(bool checked);
    void on_classBox_currentIndexChanged(int index);
    void on_utteranceBox_currentIndexChanged(int index);
    void updateAudioLevels(QAudioBuffer audioBuffer);
    void on_showMagButton_toggled(bool checked);
    void sensorDisplayClosed();
    void saveEMF();
    void on_trialBox_currentIndexChanged(int index);


private:
    void loadConfig();
    void loadCalibration(QString calibFilename);
    void setupExperiment();
    void loadExperimentFile(QString experimentFile);
    void setAudio();
    void beginTrial();
    void stopTrial();
    void setFilePath();
    CImg<double> loadVector(string myString);
    CImg<double> loadMatrix(string myString);

/* ****************************
 *              Signals
 * *************************** */
signals:
    void save(bool);
    void dispVideo(bool);
    void fileName(QString);
    void stopRecording();

};

#endif // TTS_GUI_H
