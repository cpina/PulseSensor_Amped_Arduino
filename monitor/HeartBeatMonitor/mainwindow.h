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

protected:
    void resizeEvent(QResizeEvent* event);

private slots:
    void updateData();

private:
    void drawLinePoints(QPainter *painter, FileReader::HeartBeat heartBeat1, FileReader::HeartBeat heartBeat2, quint64 minimumTimestamp, quint64 maximumTimestamp, const QColor &color);
    void updateInformation();
    void updateGraphic();
    void drawVerticalScale(QPainter* painter);
    int yPositionForBpm(QPainter* painter, int bpm);

    Ui::MainWindow *ui;
    FileReader m_fileReader;
};

#endif // MAINWINDOW_H
