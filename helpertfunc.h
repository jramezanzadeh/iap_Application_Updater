#ifndef HELPERTFUNC_H
#define HELPERTFUNC_H
#include <QByteArray>

class HelpertFunc
{
public:
    HelpertFunc();

    static QByteArray calcMac(QByteArray data);
};

#endif // HELPERTFUNC_H
