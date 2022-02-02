/**
 * @file server.cc
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../common/common.h"

UserManager user_svr;
DbManager db_svr;

void Test()
{
	char user_name[] = "test";
	char password[] = "123456";
	int ret = user_svr.CreateUser(user_name, password, 0, 0);
}

int main() {
    db_svr.Init();
    user_svr.Init(&db_svr);
    user_svr.Start();

    Test();

    int server_sock, clnt_sock;
    server_sock = SocketInit();
    clnt_sock = SocketAccept(clnt_sock, server_sock);
    
    return 0;
}