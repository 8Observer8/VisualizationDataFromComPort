#include "Receiver.h"

Receiver::Receiver( const QString &portName,
                    QSerialPort::BaudRate baudRate,
                    QSerialPort::DataBits dataBits,
                    QSerialPort::Parity parity,
                    QSerialPort::StopBits stopBits,
                    QSerialPort::FlowControl flowControl ) :
    m_portName( portName ),
    m_baudRate( baudRate ),
    m_dataBits( dataBits ),
    m_parity( parity ),
    m_stopBits( stopBits ),
    m_flowControl( flowControl )
{
}

Receiver::Receiver( const Receiver &receiver )
{
    this->m_portName = receiver.getPortName();
    this->m_baudRate = receiver.getBaudRate();
    this->m_dataBits = receiver.getDataBist();
    this->m_parity = receiver.getParity();
    this->m_stopBits = receiver.getStopBits();
    this->m_flowControl = receiver.getFlowControl();
}

Receiver::~Receiver()
{
    m_serialPort.close();
}

void Receiver::run( ) throw( PortError )
{
    m_serialPort.setPortName( m_portName );

    if ( !m_serialPort.open( QIODevice::ReadOnly ) ) {
        throw PortError( m_portName.toStdString() );
    }

    m_serialPort.setBaudRate( m_baudRate );
    m_serialPort.setDataBits( m_dataBits );
    m_serialPort.setParity( m_parity );
    m_serialPort.setStopBits( m_stopBits );
    m_serialPort.setFlowControl( m_flowControl );

    connect( &m_serialPort, SIGNAL( readyRead( ) ),
             this, SLOT( slotReadyRead( ) ) );
}

QString Receiver::getPortName() const
{
    return m_portName;
}

QSerialPort::BaudRate Receiver::getBaudRate() const
{
    return m_baudRate;
}

QSerialPort::DataBits Receiver::getDataBist() const
{
    return m_dataBits;
}

QSerialPort::Parity Receiver::getParity() const
{
    return m_parity;
}

QSerialPort::StopBits Receiver::getStopBits() const
{
    return m_stopBits;
}

QSerialPort::FlowControl Receiver::getFlowControl() const
{
    return m_flowControl;
}

void Receiver::slotReadyRead( )
{
    QByteArray data;
    data = m_serialPort.readAll( );
    emit signalReceivedData( data );
}
