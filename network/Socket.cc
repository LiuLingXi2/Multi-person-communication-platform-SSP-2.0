#include "../common/common.h"

int SocketInit()
{
    int ret;
	int server_sock;
    server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_sock < 0) {
#ifdef _D
        DBG(RED"[%s %s SOCKET ERROR]  socket error\n" NONE, __DATE__, __TIME__);
#endif
        return SOCKET_INIT_FAIL;
    }

    struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
    /**
     * @brief todo
     *         以下两个常数从.cfg文件中进行读取
     */
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	server_addr.sin_port = htons(8999);

	ret = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
#ifdef _D
        DBG(RED"[%s %s SOCKET ERROR] bind error\n" NONE, __DATE__, __TIME__);
#endif
        return SOCKET_BIND_FAIL;
    }

	ret = listen(server_sock, 10000);
    if (ret < 0) {
#ifdef _D
        DBG(RED"[%s %s SOCKET ERROR] listen error\n" NONE, __DATE__, __TIME__);
#endif
        return SOCKET_LISTEN_FAIL;
    }
#ifdef _D
	DBG(GREEN"[%s %s SOCK  SERVER] Socket Inited\n" NONE, __DATE__, __TIME__);
#endif
    return server_sock;
}

int SocketAccept(int clnt_sock, int server_sock)
{
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(server_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock < 0) {
#ifdef _D
        DBG(RED"[%s %s SOCKET ERROR] accept error\n" NONE, __DATE__, __TIME__);
#endif
        return SOCKET_ACCEPT_FAIL;
    }
#ifdef _D
	DBG(GREEN"[%s %s SOCK  SERVER] Socket Accepted\n" NONE, __DATE__, __TIME__);
#endif
	return clnt_sock;
}

int SocketCheckRecv(
    int clnt_sock, 
    int mess_type, 
    unsigned char recv_buffer[10244], 
    int no_recv_count)
{
	int ret = recv(clnt_sock, recv_buffer, 10244, 0);
	if (ret > 0)
	{
		mess_type = GetReqMessType(mess_type, recv_buffer);
#ifdef _S
        DBG(PINK"mess_type:%d\n" NONE, mess_type);
#endif		
		return mess_type;
	}
	else
	{
        no_recv_count++;
		if (no_recv_count > 10)
		{
			ClientClose(clnt_sock);
		}
	}
	return -1;
}

int ClientClose(int clnt_sock)
{
	// send_fail_count = 1;
#ifdef _D
	DBG(BLUE"[%s %s SOCK  SERVER] Client Disconnect and Closed\n" NONE, __DATE__, __TIME__);
#endif
	close(clnt_sock);
	// accept_flag = 0;
	return 0;
}

int SocketClose(int server_sock)
{
	close(server_sock);
	return 0;
}

int SocketSendRep(int send_size, int clnt_sock, unsigned char send_buffer[10244])
{
	int ret = send(clnt_sock, send_buffer, send_size, 0);
	if (ret < 0)
	{
		ClientClose(clnt_sock);
	}
	return 0;
}