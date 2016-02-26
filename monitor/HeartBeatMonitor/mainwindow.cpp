#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

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

MainWindow::~MainWindow()
{
    delete ui;
}
