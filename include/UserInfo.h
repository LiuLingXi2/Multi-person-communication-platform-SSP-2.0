#ifndef __USER_INFO_H__

#define __USER_INFO_H__

#include "../common/common.h"

class UserInfo {
public:
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
	
	/* 较为特殊，仅限于在对用户进行增删改的时候有效，即在函数SaveUser中使用 */
	GETSETVAR(int, db_flag)
};

#endif // !__USER_INFO_H__