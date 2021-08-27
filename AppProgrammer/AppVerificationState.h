#ifndef APPVERIFICATIONSTATE_H
#define APPVERIFICATIONSTATE_H

#include <State.h>

class AppVerificationState: public State{
public:
    AppVerificationState(UartAppProgrammer *ctx);

    // State interface
public:
    virtual void handleReceivedACK() override;
    virtual void handleReceivedNACK() override;
};

#endif // APPVERIFICATIONSTATE_H
