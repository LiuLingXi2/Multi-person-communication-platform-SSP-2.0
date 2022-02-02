#ifndef __SOCKET_H__

#define __SOCKET_H__

#include "../common/common.h"

int SocketInit();
int SocketAccept(int clnt_sock, int server_sock);


#endif // !__SOCKET_H__