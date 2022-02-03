#ifndef __MESSAGE_MANAGER_H__

#define __MESSAGE_MANAGER_H__

#include "../common/common.h"

class MessageManager {
private:
	MessageInfo messages_[256];
	GETSETVAR(int,message_count)

public:
	void Start();
	void Proc();
	void Shutdown();
	void Restart();

public:
	MessageInfo* GetMessage(int user_id,int message_id);
	MessageInfo* GetMessage(int message_id);
	int PublishMessage(MessageInfo message);
	int DeleteMessage(int message_id);
};

#endif // !__MESSAGE_MANAGER_H__