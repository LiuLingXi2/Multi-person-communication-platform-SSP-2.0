#ifndef __MY_MESS_TYPE_H__

#define __MY_MESS_TYPE_H__

int GetReqMessType(int mess_type, unsigned char recv_buffer[10244]);
int SetRspMessType(char *mess_type, unsigned char send_buffer[10244]);
#endif // !__MESS_TYPE_H__