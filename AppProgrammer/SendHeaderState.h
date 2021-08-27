#ifndef SENDHEADERSTATE_H
#define SENDHEADERSTATE_H

#include <State.h>

class SendHeaderState: public State{
public:
    SendHeaderState(UartAppProgrammer *ctx, BootLoaderProtocol *bootLoaderProtocol);

    // State interface
    virtual void handleReceivedACK() override;
};


#endif // SENDHEADERSTATE_H
