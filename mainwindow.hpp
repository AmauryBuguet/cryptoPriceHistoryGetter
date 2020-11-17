#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QComboBox>
#include <QCalendarWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>
#include <QtNetwork>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getCandleData();

private:
    QComboBox *_pairsComboBox;
    QComboBox *_timeframesComboBox;
    QCalendarWidget *_startCalendar;
    QCalendarWidget *_endCalendar;
    QPushButton *_startButton;

    std::map<QString,qint64> _intervalMap;
};
#endif // MAINWINDOW_HPP
