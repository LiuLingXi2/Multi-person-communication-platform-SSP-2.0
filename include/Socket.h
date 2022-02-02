#ifndef __SOCKET_H__

#define __SOCKET_H__

#include "../common/common.h"

int SocketInit();
int SocketAccept(int clnt_sock, int server_sock);
int SocketCheckRecv(int clnt_sock, int mess_type, unsigned char recv_buffer[10244], int no_recv_count);
int ClientClose(int clnt_sock);
int SocketClose(int server_sock);
int SocketSendRep(int send_size, int clnt_sock, unsigned char send_buffer[10244]);

#endif // !__SOCKET_H__