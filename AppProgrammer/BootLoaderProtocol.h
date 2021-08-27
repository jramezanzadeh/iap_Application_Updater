#ifndef FIRMWAREUPDATER_H
#define FIRMWAREUPDATER_H

#include <QObject>
#include <QIODevice>
#include <QTimer>
#include <QFile>
#include <QDebug>
#include <helpertfunc.h>

#define METADATA_MAGIC          0x0BADF00D
//#define APP_FILE_CONTAIN_META_DATA

class ApplicationMetaData{
public:
    ApplicationMetaData()
    {
        magic = METADATA_MAGIC;
    }

    void fromFile(QFile &file){
#ifdef APP_FILE_CONTAIN_META_DATA
        //TODO: handle errors
        appSize = file.size() - 16;
        md5 = file.read(8);
        revision = file.read(8);
#else
        appSize = file.size();
        md5 = HelpertFunc::calcMac(file.readAll());
        revision = "1.0.0";
        file.reset(); // seek to start of file

#endif
    }

    void print(){
        qDebug("mgic:%08X", magic);
        qDebug() << "revision:" << revision;
        qDebug() << "app size:" << appSize;
        qDebug() << "hash:" << md5.toHex();
    }

    QByteArray toByteArray()
    {
        QByteArray out;
        out.append(reinterpret_cast<char *>(&magic), sizeof (qint32)); //append magic number to ByteArray
        appendByteArray(out, revision, 8);                             //append revision to ByteArray
        out.append(reinterpret_cast<char *>(&appSize), sizeof (qint32));  //append size to ByteArray
        appendByteArray(out, md5, 8);                                  //append hash to ByteArray

        return out;

    }
    qint32 getMagic() const{return magic;};
    void setMagic(const qint32 &value){magic = value;};

    QByteArray getRevision() const{return revision;};
    void setRevision(const QByteArray &value){revision = value;};

    qint32 getAppSize() const{return appSize;};
    void setAppSize(const qint32 &value){appSize = value;};

    QByteArray getMd5() const{return md5;};
    void setMd5(const QByteArray &value){md5 = value;};

    QByteArray getMac() const{return mac;};
    void setMac(const QByteArray &value){mac = value;};

private:
    qint32      magic;
    QByteArray  revision;
    qint32      appSize;
    QByteArray  md5;
    QByteArray  mac;

    void appendByteArray(QByteArray &buff, QByteArray &inByteArray, int len){
        if(inByteArray.size() < len){
            buff += inByteArray;
            buff.append(len - inByteArray.size(), 0);
        }else{
            buff.append(inByteArray.data(), len);
        }
    }
};

class BootLoaderProtocol : public QObject
{
    Q_OBJECT
public:
    enum RET_RES{
        OK = 0,
        OPEN_DEVICE_FALID,
        DEVICE_IS_CLOSED,
        DEVICE_NOT_SET,
        WRITE_ERROR,
        TIME_OUT,
    };

    explicit BootLoaderProtocol(QObject *parent = nullptr);
    explicit BootLoaderProtocol(QIODevice *device);

    qint32 blockSize() const;
    void setBlockSize(const qint32 &blockSize);

    void setDevice(QIODevice *value);

public slots:
    RET_RES sendStartCommand();
    RET_RES sendHeader(ApplicationMetaData &meta);
    RET_RES sendDataBlock(QByteArray &data);

private slots:
    void handleReceivedData();
    void handleTimeout();

signals:
    void ackReceived();
    void nackReceived();
    void timeout();
private:
    QIODevice *mdevice;
    qint32     mTimeOut;
    QTimer     mTimer;
    qint32     mBlockSize;

    void init();
};

#endif // FIRMWAREUPDATER_H
