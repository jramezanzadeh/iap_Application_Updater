#include "State.h"
#include <QDebug>
#include <BootLoaderProtocol.h>
#include <UartAppProgrammer.h>

#define DBG_UNSUPPORTED_MESSAGE   qDebug() << "Unsupported Event"


State::State(UartAppProgrammer *ctx, BootLoaderProtocol *bootLoaderProtocol)
{
    mContext = ctx;
    mBootLoaderProtocol = bootLoaderProtocol;
}

void State::handleTimeout()
{
    mContext->setStatusWithMessage(UartAppProgrammer::DoneWithError, "Time out");
    mContext->setState(mContext->getIdleState());// default implementation, change state to IDLE_STATE
}

void State::handleReceivedACK()
{
    DBG_UNSUPPORTED_MESSAGE;
}

void State::handleReceivedNACK()
{
    mContext->setStatusWithMessage(UartAppProgrammer::DoneWithError, "NACK received(internal error)");
    mContext->setState(mContext->getIdleState());// default implementation, change state to IDLE_STATE
}

void State::handleStartCommand()
{
    DBG_UNSUPPORTED_MESSAGE;
}

