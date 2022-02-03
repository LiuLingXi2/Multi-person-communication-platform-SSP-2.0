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

ssp::RegReq regReq;
ssp::RegRep regRsp;

int ProtoInit() {
    regReq.set_verion(1);
    regReq.set_mess_type(401); // Registration message
    regReq.set_user_name("ping");
    regReq.set_password("87656743");
    return 0;
}

int SetReqMessType(const char *mess_type) {
    send_buffer[0] = mess_type[0];
    send_buffer[1] = mess_type[1];
    send_buffer[2] = mess_type[2];
    return 0;
}

int SendRegReq() {
    SetReqMessType("401");
    regReq.SerializeToArray(send_buffer + 3, 10240);

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

int RecvRegRsp() {
    int ret = recv(sock, recv_buffer, 10244, 0);
    if (ret > 0) {
        int mess_type = GetMessType();
        if (mess_type < 0) {
            return -1;
        }
        switch (mess_type) {
            case REG_REP:
                regRsp.ParseFromArray(recv_buffer + 3, 10240);
                printf("Received RegRsp: %s\n", regRsp.user_name().c_str());
                printf("ret: %d\n", regRsp.ret());
                printf("user_id: %d\n", regRsp.user_id());
                if (regRsp.ret() < 0) {
                    return -2;
                }
                break;
        }
    }
    return 0;
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

int main() {
    int client_on = 1;
    int n = 0;
    int ret;
    ClientSocketInit();
    while (client_on) {
        switch (n) {
            case 0: 
                ProtoInit();
                n = 1; // Sends packets to the server
                break;
            case 1: 
                ret = SendRegReq();
                if (ret == 0) {
                    n = 2; // The packet is successfully sent and received from the server
                }
                break;
            case 2: 
                ret = RecvRegRsp();
                // n = 1;
                if (ret == 0) {
                    n = 1; // The packet is successfully received and sent to the server
                }
                else {
                    if (ret == -2) {
                        // client_on = 0;
                    }
                }
                break;
            default:
                // client_on = 0;
                break;
        }
    }
}