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

    for (int bpm = 40; bpm < 180; bpm += 20)
    {
        int height = ui->graph->height();
        int width = ui->graph->width();

        p1.setX(50);
        p1.setY(height - ((bpm / 160.0) * height));

        p2.setX(width);
        p2.setY(height - ((bpm / 160.0) * height));

        painter->setPen(QPen(Qt::gray, 2));
        painter->drawLine(p1, p2);

        p1.setX(p1.x()-40);
        p1.setY(p1.y());
        painter->drawText(p1, QString::number(bpm));
    }
}

void MainWindow::drawLinePoints(QPainter* painter, FileReader::HeartBeat heartBeat1, FileReader::HeartBeat heartBeat2, quint64 minimumTimestamp, quint64 maximumTimestamp, const QColor& color)
{
    int height = ui->graph->height();
    int width = ui->graph->width();

    QPoint p1;
    QPoint p2;

    int x;

    x = (float(heartBeat1.msTimeStamp) - minimumTimestamp) / (maximumTimestamp - minimumTimestamp)*width;

    p1.setX(x);
    p1.setY(height - ((heartBeat1.bpm / 160.0) * height));

    x = (float(heartBeat2.msTimeStamp) - minimumTimestamp) / (maximumTimestamp - minimumTimestamp)*width;
    p2.setX(x);
    p2.setY(height - ((heartBeat2.bpm / 160.0) * height));

    painter->setPen(QPen(color, 2));
    painter->drawLine(p1, p2);
}

void MainWindow::updateGraphic()
{
    QHash<QString, FileReader::HeartBeatList> heartBeatsData = m_fileReader.heartBeatData();

    QPicture picture;
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
