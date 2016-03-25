#include "vfbmanager.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QSound>
#include <QtXml>


VfbManager::VfbManager(QObject *parent) : QObject(parent)
{

}

void VfbManager::setVfbFilePath(QString filePath)
{
    vfbFilePath = filePath;
}

void VfbManager::setRefRootPath(QString rootPath)
{
    refRootPath = rootPath;
}

void VfbManager::setRefOutPath(QString relativePath)
{
    refOutPath = refRootPath + "/" + relativePath;
}

void VfbManager::setSubId(int id)
{
    subId = id;
}

void VfbManager::setNumTrials(int numTrials)
{
    this->numTrials = numTrials;
}

void VfbManager::setCurrentUtter(QString utter)
{
    currentUtter = utter;
}

void VfbManager::setCurrentTrial(int trial)
{
    currentTrial = trial;
}

void VfbManager::updateXML(int selection)
{
    /*
     * Update the VFB XML file to sync the Matlab VFB GUI program
     */

    // The vfb.xml file should be in the same folder than executable
    QFile vfbFile(vfbFilePath);

    if (!vfbFile.open(QIODevice::ReadOnly| QIODevice::Text)) {
        QString msg = "ERROR: vfb.xml cannot be found.\nVerify if the file is in same directory than the executable file of this program.";
        qDebug() << msg;
        return;
    }

    // Load the XML content as a DOM tree
    QDomDocument vfbXml;
    bool vfbLoaded = vfbXml.setContent(&vfbFile);
    vfbFile.close();

    if (!vfbLoaded){
        QString msg = "ERROR: vfb.xml cannot be loaded.\nIt may probably be corrupted.\nReplace it with a properly formatted one";
        qDebug() << msg;
        return;
    }

    // Read content from DOM Tree
    QDomElement root = vfbXml.firstChildElement();

    if (selection == 0) {
        // Set general values (only once per session)
        root.firstChildElement("subject").childNodes().at(0).setNodeValue(QString::number(subId));
        root.firstChildElement("NumOfTrials").childNodes().at(0).setNodeValue(QString::number(numTrials));
        root.firstChildElement("saveScorePath").childNodes().at(0).setNodeValue(refRootPath);
        root.firstChildElement("refMode").childNodes().at(0).setNodeValue("false");
    }
    else {
        // Update changing values relative to current trial
        root.firstChildElement("word").childNodes().at(0).setNodeValue(currentUtter);
        root.firstChildElement("trial").childNodes().at(0).setNodeValue(QString::number(currentTrial));

        // Update Subject files
        QString subLocaFile     = subOutPath + "_loca.txt";
        QString subMagFile      = subOutPath + "_raw_sensor.txt";
        QString subAudioFile    = subOutPath + "_audio1.wav";
        QString subVideoFile    = subOutPath + "_video.avi";

        QDomElement subPath = root.firstChildElement("subPath");
        subPath.firstChildElement("localization").childNodes().at(0).setNodeValue(subLocaFile);
        subPath.firstChildElement("rawMag").childNodes().at(0).setNodeValue(subMagFile);
        subPath.firstChildElement("audio").childNodes().at(0).setNodeValue(subAudioFile);
        subPath.firstChildElement("video").childNodes().at(0).setNodeValue(subVideoFile);


        // Update Reference files
        QString refLocaFile     = refOutPath + "_loca.txt";
        QString refMagFile      = refOutPath + "_raw_sensor.txt";
        QString refAudioFile    = refOutPath + "_audio1.wav";
        QString refVideoFile    = refOutPath + "_video.avi";

        QDomElement refPath = root.firstChildElement("refPath");
        refPath.firstChildElement("localization").childNodes().at(0).setNodeValue(refLocaFile);
        refPath.firstChildElement("rawMag").childNodes().at(0).setNodeValue(refMagFile);
        refPath.firstChildElement("audio").childNodes().at(0).setNodeValue(refAudioFile);
        refPath.firstChildElement("video").childNodes().at(0).setNodeValue(refVideoFile);

    }

    // Save all changes back to VFB xml file
    vfbFile.open(QIODevice::WriteOnly| QIODevice::Text);
    QTextStream out(&vfbFile);
    out << vfbXml.toString();
    vfbFile.close();
}

void VfbManager::startVFBProgram()
{
    QString vfbFileLoc = QCoreApplication::applicationDirPath() + "/VisualFB.exe";
    QStringList args; args << " ";
    scoreGenProg = new QProcess(this);
    scoreGenProg->start(vfbFileLoc, args);
}

void VfbManager::playAudio()
{
    QString refAudioFile = refOutPath + "_audio1.wav";

    // Verify if reference sound file exist
    QFileInfo checkFile(refAudioFile);

    if (checkFile.exists() && checkFile.isFile()) {
        QSound::play(refAudioFile); // Play sound
    }

    else
    {
        QString errorMsg = "ERROR: Cannot found following audio file:\n" + refAudioFile;
        qDebug() << errorMsg;
    }
}

void VfbManager::setSubOutPath(QString path)
{
    subOutPath = path;
}

