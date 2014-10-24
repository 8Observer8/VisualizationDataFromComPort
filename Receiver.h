#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include <QString>
#include <QSerialPort>
#include <stdexcept>
#include <string>
#include "PortError.h"

class Receiver : public QObject {
    Q_OBJECT
public:

    Receiver( const QString &portName = QString( "COM2" ),
              QSerialPort::BaudRate baudRate = QSerialPort::Baud9600,
              QSerialPort::DataBits dataBits = QSerialPort::Data8,
              QSerialPort::Parity parity = QSerialPort::NoParity,
              QSerialPort::StopBits stopBits = QSerialPort::OneStop,
              QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl );

    Receiver( const Receiver &receiver );

    ~Receiver();

    void run( ) throw( PortError );

    QString getPortName() const;
    QSerialPort::BaudRate getBaudRate() const;
    QSerialPort::DataBits getDataBist() const;
    QSerialPort::Parity getParity() const;
    QSerialPort::StopBits getStopBits() const;
    QSerialPort::FlowControl getFlowControl() const;

signals:
    void signalReceivedData( QByteArray data );

private slots:
    void slotReadyRead( );

private:
    QSerialPort m_serialPort;
    QString m_portName;
    QSerialPort::BaudRate m_baudRate;
    QSerialPort::DataBits m_dataBits;
    QSerialPort::Parity m_parity;
    QSerialPort::StopBits m_stopBits;
    QSerialPort::FlowControl m_flowControl;
};

#endif // RECEIVER_H
