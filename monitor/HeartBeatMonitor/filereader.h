#ifndef FILEREADER_H
#define FILEREADER_H

#include <QString>
#include <QFile>
#include <QTimer>
#include <QHash>
#include <QDebug>

class FileReader : public QObject
{
    Q_OBJECT

public:
    struct HeartBeat
    {
        quint64 msTimeStamp;
        int bpm;
    };
    typedef QList<HeartBeat> HeartBeatList;

    FileReader(QObject *parent = 0);
    virtual ~FileReader();

    QHash<QString, HeartBeatList> heartBeatData() const;

private slots:
    void readFile();

signals:
    void dataUpdated();

private:
    QString findFile() const;

    QScopedPointer<QFile> m_file;
    QTimer m_timer;

    QHash<QString, HeartBeatList> m_heartBeatData;
};

QDebug operator<<(QDebug debug, const FileReader::HeartBeat &heartBeat);

#endif // FILEREADER_H
