#include "NotifySubject.h"

NotifySubject::NotifySubject(QObject *parent)
{
    Q_UNUSED(parent);

    //  默认启动线程，　只在　mainMainWindow 中　停止运行
    m_bRunFlag = true;
    start();
}

void NotifySubject::addObserver(IObserver *obs)
{
    m_observerList.append(obs);
}

void NotifySubject::removeObserver(IObserver *obs)
{
    m_observerList.removeOne(obs);
}

void NotifySubject::run()
{
    while (m_bRunFlag) {
        QList<MsgStruct> msgList;
        {
            QMutexLocker locker(&m_mutex);
            msgList = m_msgList;
            m_msgList.clear();
        }

        if (msgList.size() > 0) {
            foreach (const MsgStruct &msg, msgList) {
                NotifyObservers(msg.msgType, msg.msgContent);
            }
        }

        msleep(300);
    }
}

void NotifySubject::stopThreadRun()
{
    m_bRunFlag = false;

    terminate();    //终止线程
    wait();         //阻塞等待
}

void NotifySubject::sendMsg(const int &msgType, const QString &msgContent)
{
    QMutexLocker locker(&m_mutex);

    MsgStruct msg;
    msg.msgType = msgType;
    msg.msgContent = msgContent;

    m_msgList.append(msg);
}

void NotifySubject::NotifyObservers(const int &msgType, const QString &msgContent)
{
    foreach (IObserver *obs, m_observerList) {
        obs->dealWithData(msgType, msgContent);
    }
}
