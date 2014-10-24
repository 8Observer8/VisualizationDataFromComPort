#include <QSerialPort>
#include <QStringList>
#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    // Port Name
    QStringList portNames;
    portNames << "COM1" << "COM2" << "COM3" << "COM4" <<
                 "COM5" << "COM6" << "COM7" << "COM8";
    ui->portNameComboBox->addItems( portNames );

    // Baud Rate
    QStringList baudRates;
    baudRates << "1200" << "2400" << "4800" << "9600" <<
                 "19200" << "38400" << "57600" << "115200";
    ui->baudRateComboBox->addItems( baudRates );
    ui->baudRateComboBox->setCurrentIndex( 3 );

    // Data Bits
    ui->dataBitsComboBox->addItem( "5", QSerialPort::Data5 );
    ui->dataBitsComboBox->addItem( "6", QSerialPort::Data6 );
    ui->dataBitsComboBox->addItem( "7", QSerialPort::Data7 );
    ui->dataBitsComboBox->addItem( "8", QSerialPort::Data8 );
    ui->dataBitsComboBox->setCurrentIndex( 3 );

    // Parity
    ui->parityComboBox->addItem( "No Parity", QSerialPort::NoParity );
    ui->parityComboBox->addItem( "Even Parity", QSerialPort::EvenParity );
    ui->parityComboBox->addItem( "Odd Parity", QSerialPort::OddParity );
    ui->parityComboBox->addItem( "Space Parity", QSerialPort::SpaceParity );
    ui->parityComboBox->addItem( "Mark Parity", QSerialPort::MarkParity );
    ui->parityComboBox->setCurrentIndex( 0 );

    // Stop Bits
    ui->stopBitsComboBox->addItem( "One Stop", QSerialPort::OneStop );
    ui->stopBitsComboBox->addItem( "One And Half Stop", QSerialPort::OneAndHalfStop );
    ui->stopBitsComboBox->addItem( "Two Stop", QSerialPort::TwoStop );
    ui->stopBitsComboBox->setCurrentIndex( 0 );

    // Flow Control
    ui->flowControlComboBox->addItem( "No Flow Control", QSerialPort::NoFlowControl );
    ui->flowControlComboBox->addItem( "Hardware Control", QSerialPort::HardwareControl );
    ui->flowControlComboBox->addItem( "Software Control", QSerialPort::SoftwareControl );
    ui->flowControlComboBox->setCurrentIndex( 1 );
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_okButton_clicked()
{
    // Port Name
    QString portName = ui->portNameComboBox->currentText();

    QSerialPort::BaudRate baudRate =
            static_cast<QSerialPort::BaudRate> ( ui->baudRateComboBox->currentText().toInt() );

    // Baud Rate
    int currentIndex = ui->dataBitsComboBox->currentIndex();
    QSerialPort::DataBits dataBits = static_cast<QSerialPort::DataBits>( ui->dataBitsComboBox->itemData( currentIndex ).toInt() );

    // Parity
    currentIndex = ui->parityComboBox->currentIndex();
    QSerialPort::Parity parity = static_cast<QSerialPort::Parity>( ui->parityComboBox->itemData( currentIndex ).toInt() );

    // Stop Bits
    currentIndex = ui->stopBitsComboBox->currentIndex();
    QSerialPort::StopBits stopBits = static_cast<QSerialPort::StopBits>( ui->stopBitsComboBox->itemData( currentIndex ).toInt() );

    // FlowControl
    currentIndex = ui->flowControlComboBox->currentIndex();
    QSerialPort::FlowControl flowControl = static_cast<QSerialPort::FlowControl>( ui->flowControlComboBox->itemData( currentIndex ).toInt() );

    Receiver receiver( portName,
                       baudRate,
                       dataBits,
                       parity,
                       stopBits,
                       flowControl );

    emit signalSetSettings( receiver );

    this->close();
}

void SettingsDialog::on_cancelButton_clicked()
{
    this->close();
}
