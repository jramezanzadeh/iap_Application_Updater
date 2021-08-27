#ifndef STATE_H
#define STATE_H

#include <QDebug>

class UartAppProgrammer;
class BootLoaderProtocol;

class State{
public:
    State(UartAppProgrammer *ctx, BootLoaderProtocol *bootLoaderProtocol);

    virtual void handleTimeout();
    virtual void handleReceivedACK();
    virtual void handleReceivedNACK();
    virtual void handleStartCommand();

protected:
    UartAppProgrammer *mContext;
    BootLoaderProtocol *mBootLoaderProtocol;
};
#endif // STATE_H
