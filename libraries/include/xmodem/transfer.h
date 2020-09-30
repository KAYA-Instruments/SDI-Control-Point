/* Copyright (C) 2020 J.Luis <root@heavydeck.net>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */
#ifndef TRANSFER_H
#define TRANSFER_H

#include <QObject>
#include <QSerialPort>
#include <QThread>

class Transfer : public QThread
{
    Q_OBJECT
public:
    explicit Transfer(QObject *parent = nullptr);

    void config(
            QString serialPortName,
            qint32 baudrate,
            QString filePath
            );

    void setParity(QSerialPort::Parity parirty);
    void setStopBits(QSerialPort::StopBits stopBits);
    void SetFlowControl(QSerialPort::FlowControl flowControl);
    void setPkcsPadding(bool enabled);

    virtual ~Transfer();
    void launch();
    void cancel();

signals:
    void updateProgress(quint32);
    void transferCompleted();
    void transferFailed(QString);

protected:
    //QThread method
    void run () override;
    bool waitReadable(int timeout);

    static const quint32 timeoutRead      = 5000;
    static const quint32 timeoutFirstRead = 4000;

private:
    QSerialPort *serialPort;
    QString filePath;
    bool usePkcsPadding;
    bool cancelRequested;
};

#endif // TRANSFER_H
