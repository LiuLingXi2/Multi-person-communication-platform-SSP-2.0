#include "../common/common.h"

int SocketInit()
{
    int ret;
	int server_sock;
    server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_sock < 0) {
#ifdef _D
        DBG(RED"<socket> socket error\n" NONE);
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
        DBG(RED"<socket> bind error\n" NONE);
#endif
        return SOCKET_BIND_FAIL;
    }

	ret = listen(server_sock, 10000);
    if (ret < 0) {
#ifdef _D
        DBG(RED"<socket> listen error\n" NONE);
#endif
        return SOCKET_LISTEN_FAIL;
    }
#ifdef _D
	DBG(GREEN"Socket Inited\n" NONE);
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
        DBG(RED"<socket> accept error\n" NONE);
#endif
        return SOCKET_ACCEPT_FAIL;
    }
#ifdef _D
	DBG(GREEN"Socket Accepted\n" NONE);
#endif
	return clnt_sock;
}