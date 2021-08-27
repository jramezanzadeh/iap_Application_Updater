#ifndef IDLESTATE_H
#define IDLESTATE_H

#include <State.h>

class IdleState: public State{
public:
    IdleState(UartAppProgrammer *ctx, BootLoaderProtocol *bootLoaderProtocol);

    // State interface
    virtual void handleTimeout() override;
    virtual void handleReceivedACK() override;
    virtual void handleReceivedNACK() override;
    virtual void handleStartCommand() override;
};

#endif // IDLESTATE_H
