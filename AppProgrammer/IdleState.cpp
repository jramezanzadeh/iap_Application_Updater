#include "IdleState.h"
#include <BootLoaderProtocol.h>
#include <UartAppProgrammer.h>
#include "QDebug"

#define DBG_UNSUPPORTED_MESSAGE   qDebug() << "Unsupported Event"

IdleState::IdleState(UartAppProgrammer *ctx, BootLoaderProtocol *bootLoaderProtocol):
    State(ctx, bootLoaderProtocol)
{

}

void IdleState::handleTimeout()
{
    DBG_UNSUPPORTED_MESSAGE;
}

void IdleState::handleReceivedACK()
{
    DBG_UNSUPPORTED_MESSAGE;
}

void IdleState::handleReceivedNACK()
{
    DBG_UNSUPPORTED_MESSAGE;
}

void IdleState::handleStartCommand()
{
    if(mBootLoaderProtocol->sendStartCommand() == BootLoaderProtocol::OK ){
        mContext->setStatusWithMessage(UartAppProgrammer::StartedProgramming);
        mContext->setState(mContext->getStartState());
    }else{
        mContext->setStatusWithMessage(UartAppProgrammer::DoneWithError, "send command failed");
        qDebug() <<"start command failed";
    }

}
