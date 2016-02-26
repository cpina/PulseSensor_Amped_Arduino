#include "filereader.h"

#include <QFileInfoList>
#include <QDir>

FileReader::FileReader(QObject* parent)
    :
    QObject(parent)
{
    connect(&m_timer, SIGNAL(timeout()),
            this, SLOT(readFile()));

    m_timer.setInterval(2000);
    m_timer.start();

    readFile();
}

FileReader::~FileReader()
{

}

void FileReader::readFile()
{
    m_heartBeatData.clear();

    QString filePath = findFile();

    QFile file(filePath);
    file.open(QFile::ReadOnly);

    QTextStream textStream(&file);

    QString line;

    QString heartDevice;
    qint64 msTimeStamp;
    int bpm;
    QStringList splitLine;
    HeartBeat heartBeat;

    do
    {
        line = textStream.readLine();
        splitLine = line.split(",");
        if (splitLine.count() != 3)
        {
            break;
        }

        msTimeStamp = splitLine[0].toFloat()*100;
        bpm = splitLine[1].toInt();
        heartDevice = splitLine[2];

        heartBeat.bpm = bpm;
        heartBeat.msTimeStamp = msTimeStamp;

        m_heartBeatData[heartDevice].append(heartBeat);
    } while(!line.isNull());

    emit dataUpdated();
}

QString FileReader::findFile() const
{
    QDir dir("/home/carles/git/PulseSensor_Amped_Arduino/python");
    QFileInfoList filesInfo = dir.entryInfoList(QStringList() << "session-*.csv", QDir::AllDirs|QDir::Files);

    QStringList fileAbsolutePaths;
    Q_FOREACH(const QFileInfo& fileInfo, filesInfo)
    {
        fileAbsolutePaths.append(fileInfo.absoluteFilePath());
    }

    qSort(fileAbsolutePaths.begin(), fileAbsolutePaths.end());

    if (!fileAbsolutePaths.isEmpty())
    {
        return fileAbsolutePaths.last();
    }
    else
    {
        return QString();
    }
}

QHash<QString, QList<FileReader::HeartBeat> > FileReader::heartBeatData() const
{
    return m_heartBeatData;
}

QDebug operator<<(QDebug debug, const FileReader::HeartBeat &heartBeat)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << '(' << heartBeat.msTimeStamp << ", " << heartBeat.bpm << ')';

    return debug;
}
