#ifndef __MESSAGE_INFO_H__

#define __MESSAGE_INFO_H__

#include "../common/common.h"

class MessageInfo {
	GETSETVAR(int,publisher)
	GETSETVAR(int,message_id)
	GETSETSTR(10240,content)
	GETSETVAR(int,publish_time)
};


#endif // !__MESSAGE_INFO_H__