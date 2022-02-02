#include "../common/common.h"

int GetReqMessType(int mess_type, unsigned char recv_buffer[10244]) {
    return ((recv_buffer[0] - '0') * 100 + (recv_buffer[1] - '0') * 10 + recv_buffer[2] - '0');
}

int SetRspMessType(char *mess_type, unsigned char send_buffer[10244])
{
	if (strlen(mess_type) >= 3)
	{
		send_buffer[0] = mess_type[0];
		send_buffer[1] = mess_type[1];
		send_buffer[2] = mess_type[2];
	}
	return 0;
}
