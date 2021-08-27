#include "SendBlocksState.h"
#include <UartAppProgrammer.h>
#include <BootLoaderProtocol.h>

SendBlocksState::SendBlocksState(UartAppProgrammer *ctx, BootLoaderProtocol *bootLoaderProtocol):
    State(ctx, bootLoaderProtocol)
{

}

void SendBlocksState::handleReceivedACK()
{
    if(mContext->getRemaingBlocks() > 0){
        QByteArray block = mContext->getNextBlock();
        if(mBootLoaderProtocol->sendDataBlock(block) != BootLoaderProtocol::OK){
            qDebug() <<"send block failed";
            mContext->setStatusWithMessage(UartAppProgrammer::DoneWithError, "send block failed");
            mContext->setState(mContext->getIdleState());
        }else{
            mContext->setStatusWithMessage(UartAppProgrammer::Programming);
        }
    }else{ //end of programming
        qDebug() <<"send last block successfully";
        mContext->setStatusWithMessage(UartAppProgrammer::Programming);
        mContext->setState(mContext->getAppVerificationState());
    }
}

