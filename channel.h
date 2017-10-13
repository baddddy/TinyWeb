/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---Configer Class---
*We need to upgrade this class
*in order to handle config file error
****************************************
*
*/

#ifndef CHANNEL_H
#define CHANNEL_H

#include "callback.h"
#include "time.h"

#include <sys/epoll.h>
#include <boost/function.hpp>

class EventLoop;

static int ChannelFlag_New = -1;
static int ChannelFlag_Added = -2;
static int ChannelFlag_Deleted = -3;

static int kNoneEvent = 0;
static int kReadEvent = EPOLLIN | EPOLLPRI;
static int kWriteEvent = EPOLLOUT;

typedef boost::function<void()> callback;
typedef boost::function<void(Time)> readCallback;

class Channel
{
private:
  EventLoop *m_pEventLoop;
  int m_nFd;
  readCallback m_fReadCallback;
  callback m_fWriteCallback;
  callback m_fErrorCallback;
  callback m_fCloseCallback;

  int m_nEvent;
  int m_nREvent;
  int m_nFlag; //used by EPoller::m_nEvents;
  void m_fUpdate();

public:
  Channel(EventLoop *, int fd);

  bool isNoneEvent() { return 0 == m_nEvent; }
  int getFd() { return m_nFd; }
  int getFlag() { return m_nFlag; }
  void setFlag(int f) { m_nFlag = f; }

  int getEvent() { return m_nEvent; }
  void handleEvent(Time);

  void setREvent(int event) { m_nREvent = event; }

  void setReadCallback(const readCallback &c) { m_fReadCallback = c; };
  void setWriteCallback(const callback &c) { m_fWriteCallback = c; };
  void setCloseCallback(const callback &c) { m_fErrorCallback = c; };
  void setErrorCallback(const callback &c) { m_fCloseCallback = c; };

  void enableRead()
  {
    m_nEvent |= kReadEvent;

    m_fUpdate();
  }
  void enableWrite()
  {
    m_nEvent |= kWriteEvent;
    m_fUpdate();
  }
  void enableAll()
  {
    m_nEvent |= kReadEvent;
    m_nEvent |= kWriteEvent;
    m_fUpdate();
  }

  void disableRead()
  {
    m_nEvent &= ~kReadEvent;
    m_fUpdate();
  }
  void disableWrite()
  {
    m_nEvent &= ~kWriteEvent;
    m_fUpdate();
  }
  void disableAll()
  {
    m_nEvent = kNoneEvent;
    m_fUpdate();
  }

  bool isWriting()
  {
    return m_nEvent & kWriteEvent;
  }

  ~Channel();
};

#endif