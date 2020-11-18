#include "mainwindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _intervalMap.insert({"1m",60});
    _intervalMap.insert({"5m",300});
    _intervalMap.insert({"15m",900});
    _intervalMap.insert({"1h",3600});
    _intervalMap.insert({"4h",14400});
    _intervalMap.insert({"1d",86400});
    _startCalendar = new QCalendarWidget(this);
    _startCalendar->setGridVisible(true);
    _startCalendar->setMinimumDate(QDate(2015, 1, 1));
    _startCalendar->setNavigationBarVisible(true);
    _startCalendar->setStyleSheet("QTableView{selection-background-color: green}");
    _endCalendar = new QCalendarWidget(this);
    _endCalendar->setGridVisible(true);
    _endCalendar->setMinimumDate(QDate(2015, 1, 1));
    _endCalendar->setNavigationBarVisible(true);
    _endCalendar->setStyleSheet("QTableView{selection-background-color: red}");
    _pairsComboBox = new QComboBox(this);
    _pairsComboBox->addItem("BTCUSDT");
    _pairsComboBox->addItem("ETHUSDT");
    _pairsComboBox->addItem("BNBUSDT");
    _pairsComboBox->addItem("XLMUSDT");
    _pairsComboBox->addItem("LTCUSDT");
    _pairsComboBox->addItem("ADAUSDT");
    _pairsComboBox->addItem("XRPUSDT");
    _pairsComboBox->addItem("EOSUSDT");
    _pairsComboBox->addItem("XTZUSDT");
    _pairsComboBox->addItem("LINKUSDT");
    _timeframesComboBox = new QComboBox(this);
    _timeframesComboBox->addItem("1m");
    _timeframesComboBox->addItem("5m");
    _timeframesComboBox->addItem("15m");
    _timeframesComboBox->addItem("1h");
    _timeframesComboBox->addItem("4h");
    _timeframesComboBox->addItem("1d");
    _startButton = new QPushButton("START", this);

    QWidget *widget = new QWidget(this);
    setCentralWidget(widget);

    QGridLayout *layout = new QGridLayout(widget);
    layout->addWidget(_startCalendar,0,0);
    layout->addWidget(_endCalendar,0,1);
    layout->addWidget(_pairsComboBox,1,0);
    layout->addWidget(_timeframesComboBox,2,0);
    layout->addWidget(_startButton,1,1,2,1);

    connect(_startButton, &QPushButton::clicked, this, &MainWindow::getCandleData);
}

void MainWindow::getCandleData()
{
    _startButton->setEnabled(false);

    QTime time(0,0);
    qint64 startTime = QDateTime(_startCalendar->selectedDate(), time).toMSecsSinceEpoch();
    qint64 endTime = QDateTime(_endCalendar->selectedDate(), time).toMSecsSinceEpoch();
    QString pair = _pairsComboBox->currentText();
    QString interval = _timeframesComboBox->currentText();
    qint64 currTime = startTime + 999000*_intervalMap[interval];

    QFile file("C:/Users/amo/Documents/Qt/candleData/" + pair
               + "_" + interval
               + "_" + _startCalendar->selectedDate().toString("yyyy-MM-dd")
               + "_" + _endCalendar->selectedDate().toString("yyyy-MM-dd") + ".txt");
    file.remove();
    file.open(QIODevice::WriteOnly);
    file.write("[");

    do {
        if(currTime > endTime) currTime = endTime;
        QString url = "https://fapi.binance.com/fapi/v1/klines?symbol=" + pair + "&interval=" + interval
                + "&startTime=" + QString::number(startTime, 'f',0)
                + "&endTime=" + QString::number(currTime, 'f',0);

        QNetworkRequest request((QUrl(url)));
        request.setSslConfiguration(QSslConfiguration::defaultConfiguration());

        QNetworkAccessManager manager;
        QNetworkReply *response = manager.get(request);
        response->ignoreSslErrors();
        QEventLoop event;
        connect(response, &QNetworkReply::finished, &event, &QEventLoop::quit);
        event.exec();
        QByteArray data = QJsonDocument::fromJson(response->readAll()).toJson(QJsonDocument::Compact);
        data.remove(data.size()-1, 1);
        data.remove(0, 1);
        file.write(data);
        startTime = currTime + 1000*_intervalMap[interval];
        currTime = startTime + 999000*_intervalMap[interval];
        if(startTime > endTime){
            break;
        }
        else file.write(",");
    } while (true);

    file.write("]");
    file.close();
    _startButton->setEnabled(true);
}

MainWindow::~MainWindow()
{
}

