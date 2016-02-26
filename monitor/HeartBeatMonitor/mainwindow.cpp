#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QPainter>
#include <QPicture>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&m_fileReader, SIGNAL(dataUpdated()),
            this, SLOT(updateData()));
    updateData();
}

void MainWindow::updateData()
{
    updateInformation();
    updateGraphic();
}

void MainWindow::updateInformation()
{
    QHash<QString, FileReader::HeartBeatList> heartBeats = m_fileReader.heartBeatData();

    QString information;
    Q_FOREACH(const QString& device, heartBeats.keys())
    {
        if (!information.isEmpty())
        {
            information += "<br>";
        }
        information += QString("<b>%1:</b> %2").arg(device).arg(heartBeats.value(device).last().bpm);
    }
    ui->information->setText(information);
}

void MainWindow::drawLinePoints(QPainter* painter, FileReader::HeartBeat heartBeat1, FileReader::HeartBeat heartBeat2, quint64 minimumTimestamp, quint64 maximumTimestamp, const QColor& color)
{
    int height = ui->graph->height();
    int width = ui->graph->width();

    QPoint p1;
    QPoint p2;

    int x;

    x = (float(heartBeat1.msTimeStamp) - minimumTimestamp) / (maximumTimestamp - minimumTimestamp)*width;
    qDebug() << "heartBeat1:" << heartBeat1.msTimeStamp;
    qDebug() << "minimumTimestamp:" << minimumTimestamp;
    qDebug() << "maximumTimestamp:" << maximumTimestamp;
    qDebug() << "x:" << x;
    p1.setX(x);
    p1.setY((heartBeat1.bpm / 160.0) * height);

    x = (float(heartBeat2.msTimeStamp) - minimumTimestamp) / (maximumTimestamp - minimumTimestamp)*width;
    p2.setX(x);
    p2.setY((heartBeat2.bpm / 160.0) * height);

    painter->setPen(QPen(color, 2));
    painter->drawLine(p1, p2);
}

void MainWindow::updateGraphic()
{
    QHash<QString, FileReader::HeartBeatList> heartBeatsData = m_fileReader.heartBeatData();

    QPicture picture;
    QPainter painter(&picture);

    int width = ui->graph->width();
    int height = ui->graph->height();

    Q_FOREACH(const QString& device, heartBeatsData.keys())
    {
        QColor color = Qt::black;

        if (device == "Polar")
        {
            color = Qt::red;
        }
        else if (device == "PulseSensor")
        {
            color = Qt::green;
        }

        FileReader::HeartBeatList heartBeats = heartBeatsData.value(device);

        int limit = 20;
        quint64 minimum = 99999;
        quint64 maximum = 00000;
        // Get minimum and maximum
        for (int i = heartBeats.count() - 1; i >= 0 && limit >= 0; i--, limit--)
        {
            quint64 timestamp = heartBeats.at(i).msTimeStamp;

            if (minimum > timestamp)
            {
                minimum = timestamp;
            }

            if (maximum < timestamp)
            {
                maximum = timestamp;
            }
        }

        limit = 20;
        for (int i = heartBeats.count() - 1; i > 0 && limit > 0; i--, limit--)
        {
            drawLinePoints(&painter, heartBeats.at(i), heartBeats.at(i-1), minimum, maximum, color);
        }
    }

    ui->graph->setPicture(picture);
    ui->graph->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
