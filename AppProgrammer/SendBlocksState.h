#ifndef SENDBLOCKSSTATE_H
#define SENDBLOCKSSTATE_H

#include <State.h>

class SendBlocksState: public State{
public:
    SendBlocksState(UartAppProgrammer *ctx, BootLoaderProtocol *bootLoaderProtocol);

    // State interface
    virtual void handleReceivedACK() override;
};

#endif // SENDBLOCKSSTATE_H
