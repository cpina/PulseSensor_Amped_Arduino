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
        QString color;
        if (device == "Polar")
        {
            color="red";
        }
        else
        {
            color="green";
        }
        information += QString("<b><font color=\"%1\">%2:</font></b> %3").arg(color).arg(device).arg(heartBeats.value(device).last().bpm);
    }
    ui->information->setText(information);
}

void MainWindow::drawVerticalScale(QPainter* painter)
{
    QPoint p1;
    QPoint p2;

    const int header = 0;
    const int height = painter->viewport().height() - header;
    const int width = painter->viewport().width();
    const int minimumBpm = 60;
    const int maximumBpm = 140;
    const int bpmLineIncrements = 20;
    const int bpmPixelsIncrements = height / ((maximumBpm - minimumBpm) / bpmLineIncrements);

    int lineNumber = 0;

    for (int bpm = minimumBpm; bpm <= maximumBpm; bpm += bpmLineIncrements)
    {
        p1.setX(50);
        p1.setY(yPositionForBpm(painter, bpm));

        p2.setX(width);
        p2.setY(yPositionForBpm(painter, bpm));

        painter->setPen(QPen(Qt::gray, 2));
        painter->drawLine(p1, p2);

        p1.setX(p1.x()-40);
        p1.setY(p1.y());
        painter->drawText(p1, QString::number(bpm));

        lineNumber++;
    }
}

int MainWindow::yPositionForBpm(QPainter *painter, int bpm)
{
    const int header = 50;
    const int height = painter->viewport().height() - header;
    const int minimumBpm = 60;
    const int maximumBpm = 140;

    //int position = (bpm-minimumBpm)*height;

    int totalBpm = maximumBpm - minimumBpm;

    float percentage = (float(bpm)-minimumBpm) / totalBpm;
    int position = height - (percentage * height) + header;

    return position;
}

void MainWindow::drawLinePoints(QPainter* painter, FileReader::HeartBeat heartBeat1, FileReader::HeartBeat heartBeat2, quint64 minimumTimestamp, quint64 maximumTimestamp, const QColor& color)
{
    int width = painter->viewport().width();

    QPoint p1;
    QPoint p2;

    int x;

    x = (float(heartBeat1.msTimeStamp) - minimumTimestamp) / (maximumTimestamp - minimumTimestamp)*width;

    p1.setX(x);
    p1.setY(yPositionForBpm(painter, heartBeat1.bpm));

    x = (float(heartBeat2.msTimeStamp) - minimumTimestamp) / (maximumTimestamp - minimumTimestamp)*width;
    p2.setX(x);
    p2.setY(yPositionForBpm(painter, heartBeat2.bpm));

    painter->setPen(QPen(color, 2));
    painter->drawLine(p1, p2);
}

void MainWindow::updateGraphic()
{
    QHash<QString, FileReader::HeartBeatList> heartBeatsData = m_fileReader.heartBeatData();

    QPicture picture;
    picture.setBoundingRect(QRect(ui->graph->rect()));
    QPainter painter(&picture);

    Q_FOREACH(const QString& device, heartBeatsData.keys())
    {
        qDebug() << "Device" << device;
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

        drawVerticalScale(&painter);
    }

    ui->graph->setPicture(picture);
    ui->graph->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    updateGraphic();
}
