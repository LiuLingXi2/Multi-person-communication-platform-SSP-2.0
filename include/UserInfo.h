#ifndef __USER_INFO_H__

#define __USER_INFO_H__

#include "../common/common.h"

class UserInfo {
private:
	GETSETVAR(int, user_id)
	GETSETSTR(256, user_name)
	GETSETSTR(256, nick_name)
	GETSETVAR(int, reg_time)
	GETSETVAR(int, from)
	GETSETVAR(int, login_time)
	GETSETVAR(int, last_login_time)
	GETSETVAR(int, fresh_time)
	GETSETSTR(256, password)
	GETSETVAR(int, logout_time)
};

#endif // !__USER_INFO_H__