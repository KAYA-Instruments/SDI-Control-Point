/* Copyright (C) 2020 J.Luis <root@heavydeck.net>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */
#include "libraries/include/xmodem/transfer.h"
#include "libraries/include/xmodem/crc16-xmodem.h"
#include <QSerialPortInfo>
#include <QIODevice>
#include <QDebug>
#include <QFile>
#include <QByteArray>

#define XMODEM_ACK  ((char)0x06)
#define XMODEM_NACK ((char)0x15)
#define XMODEM_CRC  'C'
#define XMODEM_SOH  ((char)0x01) // 128 byte data payload
#define XMODEM_STX  ((char)0x02) // 1024 byte data payload
#define XMODEM_EOT  ((char)0x04)

#define XMODEM_CAN  ((char)0x18)

static char xmodem_sum(const QByteArray &ba){
    char rv = 0;
    for(char b : ba){
        rv = rv + b;
    }
    return rv;
}

Transfer::Transfer(
        QString serialPortName,
        qint32 baudrate,
        QString filePath,
        QObject *parent
        )  : QThread(parent)
{
    qDebug() << __FILE__ << __LINE__ << "--" << __func__;
    this->cancelRequested = false;
    this->filePath = filePath;
    this->serialPort = nullptr;
    this->usePkcsPadding = false;
    //Find the serial port by name
    for(QSerialPortInfo &port_info : QSerialPortInfo::availablePorts()){
        if(port_info.portName() == serialPortName){
            this->serialPort = new QSerialPort(port_info, this);
        }
    }
    //If port found, configure it
    if(this->serialPort){
        this->serialPort->setBaudRate(baudrate);
        this->serialPort->setDataBits(QSerialPort::DataBits::Data8); //<-- Always 8

        //ToDo: These serial parameters should come from ui instead of hardcoded values
        this->serialPort->setParity(QSerialPort::Parity::NoParity);
        this->serialPort->setStopBits(QSerialPort::StopBits::OneStop);
        this->serialPort->setFlowControl(QSerialPort::FlowControl::NoFlowControl); //<-- Either NONE or HARWARE
    }
}

void Transfer::setParity(QSerialPort::Parity parirty){
    this->serialPort->setParity(parirty);
}

void Transfer::setStopBits(QSerialPort::StopBits stopBits){
    this->serialPort->setStopBits(stopBits);
}

void Transfer::SetFlowControl(QSerialPort::FlowControl flowControl){
    this->serialPort->setFlowControl(flowControl);
}

void Transfer::setPkcsPadding(bool enabled){
    this->usePkcsPadding = enabled;
}

Transfer::~Transfer(){
    qDebug() << __FILE__ << __LINE__ << "--" << __func__;
}

void Transfer::launch(){
    qDebug() << __FILE__ << __LINE__ << "--" << __func__;

    //Launch worker thread
    this->start();
}

void Transfer::cancel(){
    qDebug() << __FILE__ << __LINE__ << "--" << __func__;
    emit transferFailed(tr("Transfer cancelled"));
}


bool  Transfer::waitReadable(int timeout){

    const int timeoutStep = 10;
    int currTimeout = timeoutStep;

    do{
        if((this->serialPort->waitForReadyRead(currTimeout))
           ||
           (this->serialPort->bytesAvailable() > 0))
        {
            return true;
        }

        currTimeout += timeoutStep;
    }while(currTimeout < (timeout / 2));

    return false;
}

//The bulk of the work goes here
void Transfer::run(){
    qDebug() << __FILE__ << __LINE__ << "--" << __func__;

    //Reset progress
    emit updateProgress(0.0f);

    //Open serial port
    this->serialPort->open(QIODevice::ReadWrite);
    if(!this->serialPort->isOpen()){
        emit transferFailed(tr("Unable to open port: ") + this->serialPort->portName());
        this->cancelRequested = true;
    }

    //Open input file
    QFile *in_file = new QFile(this->filePath);
    if(in_file){
        in_file->open(QIODevice::ReadOnly);
        if(!in_file->isOpen()){
            emit transferFailed(tr("Unable to open file: ") + this->filePath);
            this->cancelRequested = true;
        }
    }
    else{
        emit transferFailed(tr("Unable to open QFile: ") + this->filePath);
        this->cancelRequested = true;
    }

    //If cancel requested at this stage, just cleanup early and return
    if(cancelRequested){
        if(in_file){ delete in_file; }
        this->serialPort->close();
        return;
    }

    const qint64 sPacketSize = 128; // 128/1024 remark: 1024 not working

    // enable FIRMWARE update
    char byteBuf[] = "firmware\r";
    this->serialPort->write(byteBuf, (qint64)strlen(byteBuf));
    this->serialPort->waitForBytesWritten();

    // this->serialPort->waitForReadyRead(this->timeoutRead);
    waitReadable(this->timeoutRead);
    this->serialPort->readAll(); // reset read buffer

    //Initialize transfer status
    quint32 current_packet = 1;
    bool use_crc = true;

    //Transfer loop
    bool transferComplete = false;
    bool sendPkcsPacket = ((in_file->size() % sPacketSize) == 0) && this->usePkcsPadding;
    bool pkcsPacketSent = false;
    do{
        //Wait for the first character from recipient
        char status_char = '\0';
        if(waitReadable(this->timeoutFirstRead)){// this->serialPort->waitForReadyRead(this->timeoutFirstRead)){
            this->serialPort->read(&status_char, 1);
            //Received a byte
            if(status_char == XMODEM_CRC){
                use_crc = true;
                status_char = XMODEM_NACK;
            }
            else if(status_char == XMODEM_NACK){
                use_crc = false;
            }
            else{
                emit transferFailed(tr("Incorrect start of XMODEM transmission (0x") + QString::number(status_char,16) + ")");
                this->cancelRequested = true;
                continue; //<-- Will cleanup and return
            }
        }
        else{
            //Timeout
            emit transferFailed(tr("Timeout"));
            this->cancelRequested = true;
            continue; //<-- Will cleanup and return
        }

        //The proper, real, main XMODEM loop
        while(!cancelRequested && !transferComplete){
            QByteArray packet;

            if(!in_file->atEnd())
            {
                //Packet header
                if(1024 == sPacketSize)
                {
                    packet.append(XMODEM_STX);
                }
                else
                {
                   packet.append(XMODEM_SOH);
                }
                packet.append(current_packet & 0xFF);
                packet.append(255U - (current_packet & 0xFF));

                //Payload
                QByteArray payload = in_file->read(sPacketSize);
                if (payload.length() < sPacketSize){
                    int padding = sPacketSize - payload.length();
                    char pad_byte = padding & 0xFF;

                    //Padding of half-full packets will be performed using the PKCS#7
                    //method of filling the pachet with the value fo the gap size.
                    //If the file to be transfered is an even split of sPacketSize bytes *and*
                    //PKCS#7 flag is enabled (this->usePkcsPadding) an aditional sPacketSize byte packet
                    //of the number sPacketSize repeated all over it *must* be sent as to guarantee
                    //padding is always present.
                    while(padding){
                        payload.append((char)pad_byte);
                        padding--;
                    }
                }
                packet.append(payload);

                //Checksum
                if(use_crc){
                    uint16_t packet_crc = crc_init();
                    packet_crc = crc_update(packet_crc, payload.data(), payload.size());
                    packet_crc = crc_finalize(packet_crc);

                    //Add CRC, big endian.
                    packet.append((packet_crc >> 8) & 0xFF);
                    packet.append(packet_crc & 0xFF);
                }
                else{
                    packet.append(xmodem_sum(packet));
                }
            }
            else if (sendPkcsPacket){
                //Packet header
                if(1024 == sPacketSize)
                {
                    packet.append(XMODEM_STX);
                }
                else
                {
                   packet.append(XMODEM_SOH);
                }

                packet.append(current_packet & 0xFF);
                packet.append(255U - (current_packet & 0xFF));
                //Payload
                const uint8_t pad_byte = 128U;
                uint16_t cksum = 0; //Same for CRC and checksum
                for(int i = 0; i < sPacketSize; i++)
                {
                    packet.append(pad_byte);
                    if(use_crc){
                        cksum = crc_update(cksum, &pad_byte, 1);
                    }
                    else{
                        cksum += pad_byte;
                    }
                }
                //Checksum
                if(use_crc){
                    packet.append((cksum >> 8) & 0xFF);
                    packet.append(cksum & 0xFF);
                }
                else{
                    packet.append(cksum & 0xFF);
                }

                //Signal we sent the packet
                pkcsPacketSent = true;
            }
            else{
                //Transfer complete!
                packet.append(XMODEM_EOT);
                packet.append(XMODEM_EOT);
                packet.append(XMODEM_EOT);
                transferComplete = true;
            }

            this->serialPort->readAll(); // reset read buffer

            /*char* pPacket = packet.data();
            for(int iPacket = 0; iPacket < packet.size(); iPacket += 128)
            {
                int lenPacket = std::min<int>(packet.size() - iPacket, 128);
                this->serialPort->write(pPacket, (qint64)lenPacket);
                // this->serialPort->write(packet);
                this->serialPort->waitForBytesWritten();

                pPacket += 128;
                msleep(100);
            }*/
            this->serialPort->write(packet);
            this->serialPort->waitForBytesWritten();

            //Read receiver response

            // bool isDataReady = this->serialPort->waitForReadyRead(200);
            // if(isDataReady || (this->serialPort->bytesAvailable() > 0)){
            if(waitReadable(1000)){
                this->serialPort->read(&status_char, 1);
                switch(status_char)
                {
                case XMODEM_ACK:
                {
                    current_packet++;

                    //If we just sent the PKCS packet, set the sendPkcsPacket
                    //flag to false so next "packet" is the end of transfer.
                    if(pkcsPacketSent){
                        sendPkcsPacket = false;
                    }
                    break;
                }
                case XMODEM_CAN:
                {
                    emit transferFailed(tr("Status terminated by device"));
                    cancelRequested = true;
                    break;
                }
                default:
                {
                    //Seek only needed within file packets. PKCS padding needs
                    //no call to seek()
                    if(!pkcsPacketSent){
                        in_file->seek((current_packet - 1) * sPacketSize);
                    }
                    break;
                }
                };
            }
            else{
                emit transferFailed(tr("Status timeout"));
                cancelRequested = true;
            }

            //Update with transfer progress
            emit updateProgress( ((current_packet - 1) * (float)sPacketSize) / ((float) in_file->size()) );

        }
    }while(0); //Shameless goto bait

    //Transfer completed, do cleanup
    if(in_file){
        delete in_file;
    }
    this->serialPort->close();
    emit updateProgress(1.0f);
    if(transferComplete && !cancelRequested){
        emit transferCompleted();
    }
}
