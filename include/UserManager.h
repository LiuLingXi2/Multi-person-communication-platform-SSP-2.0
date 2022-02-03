#ifndef __USER_MANAGER_H__

#define __USER_MANAGER_H__

#include "../common/common.h"

/**
 * @brief This class is the UserManager server, which handles user information
 *
 */

class UserManager
{
private:
	UserInfo users_[10240];
	GETSETVAR(int, user_count)
	GETSETVAR(int, reg_num)
	DbManager *db_svr_; // todo

public:
	int Init(DbManager *db_svr); // Get data from the database
	int Start();
	int Proc();
	int Shutdown();
	int Restart();

public:
	int ShowAll();
	UserInfo *GetUser(int user_id);
	int CheckExist(int user_id);
	int CreateUser(const char *user_name, const char *pswd, int from, int time_now);
	int DeleteUser(int user_id);
	int SaveUsers();
	int LoginCheck(const char *user_name, const char *user_pswd);
	int UpdateUserLogoutTime(int user_id, int time_now);
	int GetUserIdByUserName(const char *user_name);
	int UpdateUserLoginTime(int user_id, int time_now);
	int UpdateUserFreshTime(int user_id, int time_now);
};

#endif // !__USER_MANAGER_H__