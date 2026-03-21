#ifndef _IMESSAGEHANDLER_H
#define _IMESSAGEHANDLER_H
#include <iostream>
/*业务层*/
class IMessageHandler{
public:
    virtual void onMessage(int client_fd,const char* buffer,size_t length)=0;
    virtual void onDisconnect(int client_fd)=0;
    virtual ~IMessageHandler()=default;
};


/*游戏业务处理 */
class GameMessasgeHandler:public IMessageHandler{
private:

public:
    GameMessasgeHandler();
    void onMessage(int client_fd,const char* buffer,size_t length) override;
    void onDisconnect(int client_fd) override;
};
#endif
