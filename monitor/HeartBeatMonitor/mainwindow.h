#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "filereader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateData();

private:
    void drawLinePoints(QPainter *painter, FileReader::HeartBeat heartBeat1, FileReader::HeartBeat heartBeat2, quint64 minimumTimestamp, quint64 maximumTimestamp, const QColor &color);
    void updateInformation();
    void updateGraphic();
    void drawVerticalScale(QPainter* painter);
    int yPositionForBpm(int bpm);

    Ui::MainWindow *ui;
    FileReader m_fileReader;
};

#endif // MAINWINDOW_H
