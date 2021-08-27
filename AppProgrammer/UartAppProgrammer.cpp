#include "UartAppProgrammer.h"

#include <QSerialPortInfo>
#include "QDebug"

UartAppProgrammer::UartAppProgrammer(QObject *parent) : QAbstractListModel(parent),
    mIdleState(this, &mBootLoaderProtocol), mStartState(this, &mBootLoaderProtocol),
    mSendHeaderState(this, &mBootLoaderProtocol), mSendBlocksState(this, &mBootLoaderProtocol),
    mAppVerificationState(this)
{
    setConnections();
    updateAvailablePortsList();
}

UartAppProgrammer::UartAppProgrammer(const QString &fileName, const QString &portName):
    mIdleState(this, &mBootLoaderProtocol), mStartState(this, &mBootLoaderProtocol),
    mSendHeaderState(this, &mBootLoaderProtocol), mSendBlocksState(this, &mBootLoaderProtocol),
    mAppVerificationState(this)
{
    setConnections();
    setFileName(fileName);
    setUartPort(portName);
    updateAvailablePortsList();
}

UartAppProgrammer::~UartAppProgrammer()
{
    if(mfile.isOpen())
        mfile.close();
}


QString UartAppProgrammer::getFileName() const
{
    return mfile.fileName();
}

void UartAppProgrammer::setFileName(const QString &value)
{
    QUrl url(value);
    qDebug() << url.path();
    mfile.setFileName(url.path());
}

void UartAppProgrammer::setUartPort(const QString &value, qint32 baudRate)
{
    // TODO: add a method to set all port's setting
    mSerial.setBaudRate(baudRate);
    mSerial.setPortName(value);
}

void UartAppProgrammer::startProgramming(const QString fileName, QString portName, qint32 baudRate)
{
    setFileName(fileName);
    if(!mfile.open(QFile::ReadOnly)){
        setStatusWithMessage(DoneWithError, "open file failed");
        return;
    }
    //calc metaData
    metaData.fromFile(mfile);
    metaData.print();

    calcBlocksCount();
    if(mTotalBlocks == 0){
        setStatusWithMessage(DoneWithError, "file is empty");
        return;
    }

    setUartPort(portName, baudRate);
    mBootLoaderProtocol.setDevice(&mSerial);
    mState->handleStartCommand();
}

void UartAppProgrammer::setStatusWithMessage(const UartAppProgrammer::Status &status, const QString &msg)
{
    mErrorMessage = msg;
    mStatus = status;
    emit statusChanged(mStatus);

}

void UartAppProgrammer::updateAvailablePortsList()
{
    QList<QSerialPortInfo> listOfPorts = QSerialPortInfo::availablePorts();
    emit beginResetModel();
    mPortNames.clear();
    foreach(QSerialPortInfo port, listOfPorts){
        if(port.portName() != "ttyS0")
            mPortNames << port.portName();
    }
    emit endResetModel();
}

// TODO: move it to helper file that contain general helper functions
QString UartAppProgrammer::urlToFileName(QString urlString)
{
    QUrl url(urlString);
    return url.fileName();
}

BootLoaderProtocol* UartAppProgrammer::getBootLoaderProtocol()
{
    return &mBootLoaderProtocol;
}

QByteArray UartAppProgrammer::getNextBlock()
{
    if(mRemaingBlocks > 0){
        mRemaingBlocks -= 1;
        return mfile.read(mBootLoaderProtocol.blockSize());
    }
    return QByteArray();
}

qint32 UartAppProgrammer::getTotalBlocks() const
{
    return mTotalBlocks;
}

qint32 UartAppProgrammer::getRemaingBlocks() const
{
    return mRemaingBlocks;
}

void UartAppProgrammer::setConnections()
{
    mState = &mIdleState;
    connect(&mBootLoaderProtocol, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    connect(&mBootLoaderProtocol, SIGNAL(ackReceived()), this, SLOT(handleReceivedACK()));
    connect(&mBootLoaderProtocol, SIGNAL(nackReceived()), this, SLOT(handleReceivedNACK()));

}

void UartAppProgrammer::calcBlocksCount()
{
    mTotalBlocks  = metaData.getAppSize() + mBootLoaderProtocol.blockSize() - 1;
    mTotalBlocks /= mBootLoaderProtocol.blockSize();
    mRemaingBlocks = mTotalBlocks;
    qDebug() << "total blk:" << mTotalBlocks << ", "
             << "remaining Blocks:" << mRemaingBlocks << ", "
             << "blk size:" << mBootLoaderProtocol.blockSize();

}

void UartAppProgrammer::setState(State *value)
{
    mState = value;
}

State* UartAppProgrammer::getIdleState()
{
    return &mIdleState;
}

State* UartAppProgrammer::getStartState()
{
    return &mStartState;
}

State* UartAppProgrammer::getSendHeaderState()
{
    return &mSendHeaderState;
}

State* UartAppProgrammer::getSendBlocksState()
{
    return &mSendBlocksState;
}

State* UartAppProgrammer::getAppVerificationState()
{
    return &mAppVerificationState;
}

void UartAppProgrammer::handleTimeout()
{
    mState->handleTimeout();
}

void UartAppProgrammer::handleReceivedACK()
{
    mState->handleReceivedACK();
}

void UartAppProgrammer::handleReceivedNACK()
{
    mState->handleReceivedNACK();
}


int UartAppProgrammer::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return mPortNames.size();
}

QVariant UartAppProgrammer::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == NameRole)
        return QVariant::fromValue(mPortNames.at(index.row()));
    return QVariant();
}

QHash<int, QByteArray> UartAppProgrammer::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";

    return roles;
}

qreal UartAppProgrammer::getProgrammedBlocksPercentage()
{
    if(mTotalBlocks == 0)
        return 0;
    return (qreal)(mTotalBlocks - mRemaingBlocks)/(qreal)mTotalBlocks;
}
