#include "AppVerificationState.h"
#include <BootLoaderProtocol.h>
#include <UartAppProgrammer.h>
#include <QDebug>

AppVerificationState::AppVerificationState(UartAppProgrammer *ctx):State(ctx, nullptr)
{

}

void AppVerificationState::handleReceivedACK()
{
    qDebug() << "Application is Verified";
    mContext->setStatusWithMessage(UartAppProgrammer::DoneSuccessfully);
    mContext->setState(mContext->getIdleState());
}

void AppVerificationState::handleReceivedNACK()
{
    qDebug() << "Application verification failed";
    mContext->setStatusWithMessage(UartAppProgrammer::DoneWithError, "app verification failed");
    mContext->setState(mContext->getIdleState());
}

