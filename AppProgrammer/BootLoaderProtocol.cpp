#include "BootLoaderProtocol.h"
#include <QDebug>

#define START_TRANSFER_CMD      0xD4
#define START_BLOCK_CMD         0xD6
#define ACK     				0x05
#define NACK    				0x06

#define DEFAULT_TIME_OUT        5000 /*ms*/
#define DEFAULT_BLOCK_SIZE      2048

BootLoaderProtocol::BootLoaderProtocol(QObject *parent) : QObject(parent)
{
    init();
}

BootLoaderProtocol::BootLoaderProtocol(QIODevice *device)
{
    setDevice(device);
    init();
}

BootLoaderProtocol::RET_RES BootLoaderProtocol::sendStartCommand()
{
    if(mdevice == nullptr)
        return DEVICE_NOT_SET;

    if(!mdevice->isOpen()){
        if(!mdevice->open(QIODevice::ReadWrite)){
            qDebug() << "Open Device failed";
            return OPEN_DEVICE_FALID;
        }else
            qDebug() << "Open seccessfully";
    }else
        qDebug() << "port is Open";
    //clear buffer
    mdevice->readAll();
    char cmd = START_TRANSFER_CMD;
    if(mdevice->write(&cmd, 1) != 1)
        return WRITE_ERROR;
    mTimer.start(mTimeOut); // to handle timeout
    qDebug() << "send startCMD seccessfully";
    return OK;
}

BootLoaderProtocol::RET_RES BootLoaderProtocol::sendHeader(ApplicationMetaData &meta)
{
    if(mdevice == nullptr)
        return DEVICE_NOT_SET;
    if(!mdevice->isOpen())
        return DEVICE_IS_CLOSED;
    if(mdevice->write(meta.toByteArray()) != meta.toByteArray().size())
        return WRITE_ERROR;
    mTimer.start(mTimeOut); // to handle timeout
    return OK;
}

BootLoaderProtocol::RET_RES BootLoaderProtocol::sendDataBlock(QByteArray &data)
{
    if(mdevice == nullptr)
        return DEVICE_NOT_SET;
    if(!mdevice->isOpen())
        return DEVICE_IS_CLOSED;
    if(mdevice->write(data) != data.size())
        return WRITE_ERROR;
    mTimer.start(mTimeOut); // to handle timeout
    return OK;
}

void BootLoaderProtocol::handleReceivedData()
{
    QByteArray readData = mdevice->readAll();
    for(int i = 0; i < readData.size(); i++){
        if(readData.at(i) == ACK){
            mTimer.stop();
            qDebug() << "ACK received";
            emit ackReceived();
        }else if (readData.at(i) == NACK){
            mTimer.stop();
            qDebug() << "NACK received";
            emit nackReceived();
        }else
            qDebug() << "unhandled received data[" << i << "]: " << readData.at(i);
    }
}

void BootLoaderProtocol::handleTimeout()
{
    qDebug() << "receive Timeout";
    emit timeout();
}

void BootLoaderProtocol::setDevice(QIODevice *value)
{
    if(value != nullptr){
        qDebug() << "BootLoaderProtocol set device";
        mdevice = value;
        connect(mdevice, &QIODevice::readyRead, this, &BootLoaderProtocol::handleReceivedData);
    }
}

qint32 BootLoaderProtocol::blockSize() const
{
    return mBlockSize;
}

void BootLoaderProtocol::setBlockSize(const qint32 &blockSize)
{
    mBlockSize = blockSize;
}

void BootLoaderProtocol::init()
{
    mBlockSize = DEFAULT_BLOCK_SIZE;
    mTimeOut = DEFAULT_TIME_OUT;
    mTimer.setSingleShot(true);
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
}
