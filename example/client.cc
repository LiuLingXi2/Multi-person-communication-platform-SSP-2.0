/**
 * @file client.cc
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../common/common.h"

unsigned char send_buffer[10240];
unsigned char recv_buffer[10240];

int sock;

ssp::RegReq msgRegReq1;
ssp::RegRep msgRegRep1;
ssp::RegReq msgRegReq2;
ssp::RegRep msgRegRep2;
ssp::LoginReq msgLoginReq;
ssp::LoginRep msgLoginRep;
ssp::LogoutReq msgLogoutReq;
ssp::LogoutRep msgLogoutRep;
ssp::AddFriendReq msgAddFriendReq;
ssp::AddFriendRep msgAddFriendRep;
ssp::DelFriendReq msgDelFriendReq;
ssp::DelFriendRep msgDelFriendRep;
ssp::AddBlackReq msgAddBlackReq;
ssp::AddBlackRep msgAddBlackRep;
ssp::DelBlackReq msgDelBlackReq;
ssp::DelBlackRep msgDelBlackRep;
ssp::PublishMessageReq msgPublishMessageReq;
ssp::PublishMessageRep msgPublishMessageRep;
ssp::GetPhotoReq msgGetPhotoReq;
ssp::GetPhotoRep msgGetPhotoRep;
ssp::GetMessageReq msgGetMessageReq;
ssp::GetMessageRep msgGetMessageRep;

int ProtoInit() {
    msgRegReq1.set_verion(1);
    msgRegReq1.set_mess_type(401); // Registration message
    msgRegReq1.set_user_name("ping");
    msgRegReq1.set_password("87656743");

    msgRegReq2.set_verion(1);
    msgRegReq2.set_mess_type(401); // Registration message
    msgRegReq2.set_user_name("ping1");
    msgRegReq2.set_password("87656743");

    msgAddFriendReq.set_ver(1);
    msgAddFriendReq.set_mess_type(301);
    //msgAddFriendReq.set_user_id(12355);
    //msgAddFriendReq.set_other_id(12345);

    msgDelFriendReq.set_ver(1);
    msgDelFriendReq.set_mess_type(303);
    //msgDelFriendReq.set_user_id(12355);
    //msgDelFriendReq.set_other_id(12345);

    msgPublishMessageReq.set_ver(1);
    msgPublishMessageReq.set_mess_type(201);
    // msgPublishMessageReq.set_user_id(12413);
    msgPublishMessageReq.set_content("i am here");

    return 0;
}

int SetReqMessType(const char *mess_type) {
    send_buffer[0] = mess_type[0];
    send_buffer[1] = mess_type[1];
    send_buffer[2] = mess_type[2];
    return 0;
}

int SendRegReq1() {
    SetReqMessType("401");
    msgRegReq1.SerializeToArray(send_buffer + 3, 10240);

    int size_buffer = sizeof(send_buffer);
    int ret = send(sock, send_buffer, size_buffer, 0);
    if (ret > 0) {
        return 0;
    }
    return -1;
}

int mess_type;

int GetMessType() {
    return ((recv_buffer[0] - '0') * 100 + (recv_buffer[1] - '0') * 10 + recv_buffer[2] - '0');
}

int self_id = 0, other_id = 0;
int RecvRsp() {
    int ret = recv(sock, recv_buffer, 10244, 0);
    if (ret > 0) {
        int mess_type = GetMessType();
        if (mess_type < 0) {
            return -1;
        }
        switch (mess_type) {
            case PUBLISH_MESSAGE_REP:
                msgPublishMessageRep.ParseFromArray(recv_buffer + 3, 10240);
#ifdef _D
                DBG(GREEN"[CLIENT  RECV] Received PublishMessageRep:\n" NONE);
                DBG(GREEN"[CLIENT  RECV] Ret: %d\n" NONE, msgPublishMessageRep.ret());
#endif // _D
                break;
            case ADD_FRIEND_REP:
                msgAddBlackRep.ParseFromArray(recv_buffer + 3, 10240);
#ifdef _D
                DBG(GREEN"[CLIENT  RECV] Received AddBlackRep:\n" NONE);
                DBG(GREEN"[CLIENT  RECV] Ret: %d\n" NONE, msgAddBlackRep.ret());
#endif // _D
                break;
            case DEL_FRIEND_REP:
                msgDelFriendRep.ParseFromArray(recv_buffer + 3, 10240);
#ifdef _D
                DBG(GREEN"[CLIENT  RECV] Received DelFriendRep:\n" NONE);
                DBG(GREEN"[CLIENT  RECV] Ret: %d\n" NONE, msgDelFriendRep.ret());
#endif // _D
                break;
            case LOGIN_REP:
                msgLoginRep.ParseFromArray(recv_buffer + 3, 10240);
#ifdef _D
                DBG(GREEN"[CLIENT  RECV] Received LoginRep:\n" NONE);
                DBG(GREEN"[CLIENT  RECV] Ret: %d\n" NONE, msgLoginRep.ret());
                DBG(GREEN"[CLIENT  RECV] user_id: %d\n" NONE, msgLoginRep.user_id());
#endif // _D
                break;
            case REG_REP:
                msgRegRep1.ParseFromArray(recv_buffer + 3, 10240);
#ifdef _D
                DBG(GREEN"[CLIENT  RECV] Received RegRep:\n" NONE);
                DBG(GREEN"[CLIENT  RECV] Ret: %d\n" NONE, msgRegRep1.ret());
                DBG(GREEN"[CLIENT  RECV] user_id: %d\n" NONE, msgRegRep1.user_id());
#endif // _D
                if (self_id = 0) {
                    self_id = msgRegRep1.user_id();
                    msgPublishMessageReq.set_user_id(self_id);
                }
                else if (other_id = 0) {
                    other_id = msgRegRep1.user_id();
                    msgAddFriendReq.set_user_id(self_id);
                    msgAddFriendReq.set_other_id(other_id);
                    msgDelFriendReq.set_user_id(self_id);
                    msgDelFriendReq.set_other_id(other_id);
                }
                break;
        }
        return 0;
    }
    return -1;
}

int ClientSocketInit() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8999);
    int conn_ret = connect(sock, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if (conn_ret < 0) {
        printf("connect_ret: %d\n", conn_ret);
        return conn_ret;
    }
    return 0;
}

int SendRegReq2() {
    SetReqMessType("401");
    msgRegReq2.SerializeToArray(send_buffer + 3, 10240);

    int size_buffer = sizeof(send_buffer);
    int ret = send(sock, send_buffer, size_buffer, 0);
    if (ret > 0) {
        return 0;
    }
    return -1;
}

int SendAddFriendReq1() {
    SetReqMessType("301");
    msgAddFriendReq.SerializeToArray(send_buffer +3, msgAddFriendReq.ByteSize());
    int size_buff = sizeof(send_buffer);
    int ret = send(sock, send_buffer, size_buff, 0);
    if (ret > 0) {
        return 0;
    }
    return -1;
}

int SendPublishMessageReq1() {
    SetReqMessType("201");
    msgPublishMessageReq.SerializeToArray(send_buffer +3, msgPublishMessageReq.ByteSize());
    int size_buff = sizeof(send_buffer);
    int ret = send(sock, send_buffer, size_buff, 0);
    if (ret > 0) {
        return 0;
    }
    return -1;
}

int SendDelFriendReq1() {
    SetReqMessType("303");
    msgDelFriendReq.SerializeToArray(send_buffer +3, msgDelFriendReq.ByteSize());
    int size_buff = sizeof(send_buffer);
    int ret = send(sock, send_buffer, size_buff, 0);
    if (ret > 0) {
        return 0;
    }
    return -1;
}

int main() {
    int client_on = 1;
    int n = 1;
    int ret;
    ClientSocketInit();
    ProtoInit();
    for (int i = 0; i < 10000; i ++) {
        char uname[256];
        sprintf(uname, "hao%d", 10000 + i);
        msgRegReq1.set_user_name(uname);
        SendRegReq1();
        RecvRsp();
    }
    while (client_on) {
        switch (n) {
            case 1: 
                ret = SendRegReq1();
                if (ret == 0) {
                    n ++; // The packet is successfully sent and received from the server
                }
                break;
            case 2: 
                ret = RecvRsp();
                // n = 1;
                if (ret == 0) {
                    n ++; // The packet is successfully received and sent to the server
                }
                break;
            case 3:
                ret = SendRegReq2();
                if (ret == 0) {
                    n ++;
                }
                break;
            case 4:
                ret = RecvRsp();
                // n = 1;
                if (ret == 0) {
                    n ++; // The packet is successfully received and sent to the server
                }
                break;
            case 5:
                ret = SendAddFriendReq1();
                if (ret == 0) {
                    n ++;
                }
                break;
            case 6:
                ret = RecvRsp();
                if (ret == 0) {
                    n ++;
                }
                break;
            case 7:
                ret = SendPublishMessageReq1();
                if (ret == 0) {
                    n ++;
                }
                break;
            case 8:
                ret = RecvRsp();
                if (ret == 0) {
                    n ++;
                }
                break;
            case 9:
                ret = SendDelFriendReq1();
                if (ret == 0) {
                    n ++;
                }
                break;
            case 10:
                ret = RecvRsp();
                if (ret == 0) {
                    n = 5;
                }
                break;
            default:
                client_on = 0;
                break;
        }
    }
}