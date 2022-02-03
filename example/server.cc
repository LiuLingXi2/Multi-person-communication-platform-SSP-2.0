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
RelationManager rela_svr;
DbManager db_svr;

void Test()
{

    char user_name[] = "test";
    char password[] = "123456";
    int ret = user_svr.CreateUser(user_name, password, 0, 0);
}

int main()
{
    db_svr.Init();
    user_svr.Init(&db_svr);
    user_svr.Start();

    // Test();

    int server_sock, clnt_sock;
    server_sock = SocketInit();
    clnt_sock = SocketAccept(clnt_sock, server_sock);

    char mess_type_str[10];
    unsigned char recv_buffer[10244] = {0}, send_buffer[10244] = {0};
    int server_on = 1, mess_type = -1, no_recv_count = 0, times = 0, ret;
    while (server_on)
    {
        int now = time(NULL);
        mess_type = SocketCheckRecv(clnt_sock, mess_type, recv_buffer, no_recv_count);
        if (mess_type < 0)
        {
            usleep(5000);
            continue;
        }
        times++;
#ifdef _D
        DBG(GREEN "message count: %d\n" NONE, times);
#endif
        switch (mess_type)
        { /* Equilibrium model */
        case REG_REQ:
        {
            ssp::RegReq msgRegReq; // request
            ssp::RegRep msgRegRep; // reply
            /* [0]: version [1-3]: mess_type [4~]content*/
            msgRegReq.ParseFromArray(recv_buffer + 3, 10240);
            ret = user_svr.CreateUser(
                msgRegReq.user_name().c_str(),
                msgRegReq.password().c_str(),
                msgRegReq.from(), now);
            msgRegRep.set_ret(ret);
            msgRegRep.set_user_name("liuhao");
            if (ret == SUCCESS)
            {
                int user_id = user_svr.GetUserIdByUserName(msgRegRep.user_name().c_str());
                // rela_svr
            }
            sprintf(mess_type_str, "%d", REG_REP);
            SetRspMessType(mess_type_str, send_buffer);
            ret = msgRegRep.SerializeToArray(send_buffer + 3, msgRegRep.ByteSize());
            SocketSendRep(msgRegRep.ByteSize() + 3, clnt_sock, send_buffer);
            break;
        }
        default:
#ifdef _D
            DBG(YELLOW "unknown mess_type: %d\n" NONE, mess_type);
#endif
            break;
        }
        user_svr.Proc();
        mess_type = -1;
    }
    SocketClose(server_sock);
    user_svr.Shutdown();
    return 0;
}