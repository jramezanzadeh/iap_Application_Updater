#ifndef STARTSTATE_H
#define STARTSTATE_H

#include <State.h>

class StartState: public State{
public:
    StartState(UartAppProgrammer *ctx, BootLoaderProtocol *bootLoaderProtocol);

    // State interface
    virtual void handleReceivedACK() override;
};

#endif // STARTSTATE_H
