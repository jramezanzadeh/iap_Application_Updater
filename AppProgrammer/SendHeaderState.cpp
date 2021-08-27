#include "SendHeaderState.h"
#include <UartAppProgrammer.h>
#include <BootLoaderProtocol.h>


SendHeaderState::SendHeaderState(UartAppProgrammer *ctx, BootLoaderProtocol *bootLoaderProtocol):
    State(ctx, bootLoaderProtocol)
{

}

void SendHeaderState::handleReceivedACK()
{
    if(mContext->getRemaingBlocks() > 0){
        QByteArray block = mContext->getNextBlock();
        if(mBootLoaderProtocol->sendDataBlock(block) == BootLoaderProtocol::OK){
            mContext->setStatusWithMessage(UartAppProgrammer::Programming);
            mContext->setState(mContext->getSendBlocksState()); // go to SendBlockesState
        }else{
            qDebug() <<"send first block failed";
            mContext->setStatusWithMessage(UartAppProgrammer::DoneWithError, "send block failed");
            mContext->setState(mContext->getIdleState()); // go to IdleState, if there is any error
        }
    }else{
        qDebug() <<"application's size is zero !!!!";
        mContext->setStatusWithMessage(UartAppProgrammer::DoneWithError, "app file is empty");
        mContext->setState(mContext->getIdleState());
    }
}

