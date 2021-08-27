#include "StartState.h"
#include <UartAppProgrammer.h>
#include <BootLoaderProtocol.h>


StartState::StartState(UartAppProgrammer *ctx, BootLoaderProtocol *bootLoaderProtocol):
    State(ctx, bootLoaderProtocol)
{

}

void StartState::handleReceivedACK()
{
   ApplicationMetaData &metaData = mContext->getMetaData();
   if(mBootLoaderProtocol->sendHeader(metaData) == BootLoaderProtocol::OK)
       mContext->setState(mContext->getSendHeaderState()); // go to SendHeaderState
   else{
       qDebug() <<"send Header failed";
       mContext->setStatusWithMessage(UartAppProgrammer::DoneWithError, "send header failed");
       mContext->setState(mContext->getIdleState()); // go to IdleState
   }
}
