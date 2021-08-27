#ifndef UARTAPPPROGRAMMER_H
#define UARTAPPPROGRAMMER_H

#include <QAbstractListModel>
#include <QFile>
#include <QSerialPort>
#include <IdleState.h>
#include <StartState.h>
#include <SendHeaderState.h>
#include <SendBlocksState.h>
#include <AppVerificationState.h>
#include <BootLoaderProtocol.h>
#include <qqml.h>


class UartAppProgrammer : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage)

public:
    enum UartAppProgrammerRoles {
            NameRole = Qt::UserRole + 1,
        };

    enum Status{
        StartedProgramming,
        Programming,
        DoneWithError,
        DoneSuccessfully
    };
    Q_ENUM(Status)

    explicit UartAppProgrammer(QObject *parent = nullptr);
    explicit UartAppProgrammer(const QString &fileName, const QString &portName);
    virtual ~UartAppProgrammer();

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    //QML related
    Q_INVOKABLE qreal getProgrammedBlocksPercentage();
    Q_INVOKABLE void updateAvailablePortsList();
    Q_INVOKABLE QString urlToFileName(QString urlString);
    Q_INVOKABLE void startProgramming(const QString fileName, QString portName, qint32 baudRate);
    Status status() const { return mStatus; }
    QString errorMessage() const { return mErrorMessage; }
    void setStatusWithMessage(const Status &status, const QString &msg = "");

    QString getFileName() const;
    void setFileName(const QString &value);
    void setUartPort(const QString &value, qint32 baudRate= 115200);

    void setState(State *value);
    State* getIdleState();
    State* getStartState();
    State* getSendHeaderState();
    State* getSendBlocksState();
    State* getAppVerificationState();

    BootLoaderProtocol* getBootLoaderProtocol();
    ApplicationMetaData& getMetaData(){return metaData;};

    QByteArray getNextBlock();
    qint32 getTotalBlocks() const;
    qint32 getRemaingBlocks() const;


public slots:
    void handleTimeout();
    void handleReceivedACK();
    void handleReceivedNACK();

signals:
    void statusChanged(Status status);

private:
    State                   *mState;
    IdleState               mIdleState;
    StartState              mStartState;
    SendHeaderState         mSendHeaderState;
    SendBlocksState         mSendBlocksState;
    AppVerificationState    mAppVerificationState;

    qint32                  mTotalBlocks;
    qint32                  mRemaingBlocks;
    ApplicationMetaData     metaData;
    BootLoaderProtocol      mBootLoaderProtocol;

    QFile                   mfile;
    QSerialPort             mSerial;
    Status                  mStatus;
    QString                 mErrorMessage;


    QVector<QString>        mPortNames;
    void setConnections();
    void calcBlocksCount();

};

#endif // UARTAPPPROGRAMMER_H
