#include <QDebug>
#include <QMessageBox>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "SettingsDialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_value( 0.0 )
{
    ui->setupUi(this);
    this->setFixedSize( this->size() );
    m_receiver = new Receiver;

    // Status Label
    m_statusLabel = new QLabel;
    m_statusLabel->setText( tr( "Select the COM-port" ) );
    ui->statusBar->addPermanentWidget( m_statusLabel, 1 );

    ui->plotWidget->addGraph( ); // blue line
    ui->plotWidget->graph( 0 )->setPen( QPen( Qt::blue ) );
    ui->plotWidget->graph( 0 )->setBrush( QBrush( QColor( 240, 255, 200 ) ) );
    ui->plotWidget->graph( 0 )->setAntialiasedFill( false );
//    //    ui->plotWidget->graph(0)->setChannelFillGraph(ui->plotWidget->graph(1));

    ui->plotWidget->addGraph(); // blue dot
//    ui->plotWidget->graph(2)->setPen(QPen(Qt::blue));
//    ui->plotWidget->graph(2)->setLineStyle(QCPGraph::lsNone);
//    ui->plotWidget->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);

    ui->plotWidget->xAxis->setTickLabelType( QCPAxis::ltDateTime );
    ui->plotWidget->xAxis->setDateTimeFormat( "hh:mm:ss" );
    ui->plotWidget->xAxis->setAutoTickStep( false );
    ui->plotWidget->xAxis->setTickStep( 2 );
    ui->plotWidget->axisRect()->setupFullAxesBox();

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->plotWidget->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotWidget->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plotWidget->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotWidget->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog dialog;
    dialog.setModal( true );
    connect( &dialog, SIGNAL( signalSetSettings( Receiver ) ),
             this, SLOT( slotSetSettings( Receiver ) ) );
    dialog.exec();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::runReceiver()
{
    try {
        m_receiver->run();
        connect( m_receiver, SIGNAL( signalReceivedData( QByteArray ) ),
                 this, SLOT( slotReceivedData( QByteArray ) ) );
        QString text = QString( "Port Name: %1, BaudRate: %2" ).
                arg( m_receiver->getPortName() ).arg( m_receiver->getBaudRate() );

        m_statusLabel->setText( text );
    } catch ( const PortError &e ) {
        QString message( e.what() );
        QMessageBox::information( this, tr( "Error" ), message );
        return;
    } catch( ... ) {
        QString message( "Error: unknown exception" );
        QMessageBox::information( this, tr( "Error" ), message );
        return;
    }
}

void MainWindow::slotSetSettings( const Receiver &receiver )
{
    delete m_receiver;

    m_receiver = new Receiver( receiver );

    runReceiver();
}

void MainWindow::slotReceivedData( const QByteArray &data )
{
    QString dataStr = QString( data );
    ui->textEdit->append( dataStr );

    bool ok;
    m_value = dataStr.toDouble( &ok );
    if( !ok ) {
        return;
    }
}

void MainWindow::realtimeDataSlot()
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.01) { // at most add point every 10 ms

        double value0 = qSin(key); //sin(key*1.6+cos(key*1.7)*2)*10 + sin(key*1.2+0.56)*20 + 26;
        //double value1 = qCos(key); //sin(key*1.3+cos(key*1.2)*1.2)*7 + sin(key*0.9+0.26)*24 + 26;
        // add data to lines:
        ui->plotWidget->graph(0)->addData(key, m_value);
        //ui->plotWidget->graph(1)->addData(key, value1);
        // set data of dots:
//        ui->plotWidget->graph(2)->clearData();
//        ui->plotWidget->graph(2)->addData(key, value0);
//        ui->plotWidget->graph(3)->clearData();
//        ui->plotWidget->graph(3)->addData(key, value1);
        // remove data of lines that's outside visible range:
        ui->plotWidget->graph(0)->removeDataBefore(key-8);
//        ui->plotWidget->graph(1)->removeDataBefore(key-8);
        // rescale value (vertical) axis to fit the current data:
        ui->plotWidget->graph(0)->rescaleValueAxis();
//        ui->plotWidget->graph(1)->rescaleValueAxis(true);
        lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plotWidget->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
    ui->plotWidget->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
        //    ui->statusBar->showMessage(
        //          QString("%1 FPS, Total Data points: %2")
        //          .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
        //          .arg(ui->customPlot->graph(0)->data()->count()+ui->customPlot->graph(1)->data()->count())
        //          , 0);
        lastFpsKey = key;
        frameCount = 0;
    }
}
