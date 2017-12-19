#include "singleapplication.h"
#include <QLocalSocket>

SingleApplication::SingleApplication(int &argc, char *argv[], const QString uniquekey) : QApplication(argc, argv), uniquekey_(uniquekey)
{
    //find sharememory by uniquekey.if exist,do nothing,or else create sharememory
    sharememory_.setKey(uniquekey_);
    if(sharememory_.attach()){
        isrunning_ = true;
    }
    else{
        isrunning_ = false;

        //create share memory
        if(!sharememory_.create(1)){
            qDebug() << "Unable to create single instance";
            return;
        }
        localserver_ = new QLocalServer(this);
        //do ReceiveMessage_Slot when new socket is connected
        connect(localserver_,SIGNAL(newConnection()),this,SLOT(ReceiveMessage_Slot()));
        localserver_->listen(uniquekey_);
    }
}

bool SingleApplication::get_isrunning()
{
    return isrunning_;
}

bool SingleApplication::SendMessage(const QString &message)
{
    if(!isrunning_){
        return false;
    }
    QLocalSocket localsocket(this);
    localsocket.connectToServer(uniquekey_,QIODevice::WriteOnly);
    //connect
    if(!localsocket.waitForConnected(k_timeout_)){
        qDebug() << localsocket.errorString().toLatin1();
        return false;
    }
    localsocket.write(message.toUtf8());
    //callback
    if(!localsocket.waitForBytesWritten(k_timeout_)){
        qDebug() << localsocket.errorString().toLatin1();
        return false;
    }
    localsocket.disconnectFromServer();
    return true;
}

void SingleApplication::ReceiveMessage_Slot()
{
    //get socket
    QLocalSocket* plocalsocket = localserver_->nextPendingConnection();
    //socket error
    if(!localserver_->waitForNewConnection(k_timeout_)){
        qDebug() << plocalsocket->errorString().toLatin1();
        return;
    }
    //get socket infomation
    QByteArray bytearray = plocalsocket->readAll();
    QString message = QString::fromUtf8(bytearray.constData());
    //emit signal
    emit MessageAvailable_Signal(message);
    plocalsocket->disconnectFromServer();
}
