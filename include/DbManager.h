#ifndef __DB_MANAGER_H__

#define __DB_MANAGER_H__

#include "../common/common.h"
#include "mysql/mysql.h"

class DbManager {
private:
	GETSETVAR(int, transection)	/* Database transaction */
private:
	MYSQL *conn;
	MYSQL_RES *result;
	MYSQL_ROW row;
	GETSETVAR(int, cur_user_id)
	char mysql_username[256];
	char mysql_password[256];
	char mysql_host[256];
	char mysql_db[256];

private:
	void ReadMysqlInfo();
public:
	int Init();
	int initDb(std::string host, std::string user, std::string pswd, std::string db_name);
	int execSql(std::string sql);
	
	int GetUserId();
	int GetUsersBegin();
	int GetUsersOneByOne(UserInfo *user);
	int GetUsersEnd();

	int InsertUser(UserInfo *user);
	int UpdateUser(UserInfo *user);
	int DeleteUser(UserInfo *user);
};

#endif // !__DB_MANAGER_H__