#include "../common/common.h"

/**
 * @brief Read MySQL information from the configuration file
 *
 * @return int
 */
void DbManager::ReadMysqlInfo()
{
	FILE *f = fopen("config/mysql.cfg", "r");
	fscanf(f, "%s", mysql_host);
	fscanf(f, "%s", mysql_password);
	fscanf(f, "%s", mysql_username);
	fscanf(f, "%s", mysql_db);
#ifdef _D
	DBG(GREEN "<mysql> host: %s\n" NONE, mysql_host);
	DBG(GREEN "<mysql> username: %s\n" NONE, mysql_username);
	DBG(GREEN "<mysql> password: %s\n" NONE, mysql_password);
	DBG(GREEN "<mysql> db: %s\n" NONE, mysql_db);
#endif
	fclose(f);
}

int DbManager::Init()
{
	int ret = initDb("127.0.0.1", "test", "123456", "ssp");
	set_transection(0);
	result = NULL;
#ifdef _D
	DBG(GREEN"<mysql> DbManager Init\n" NONE);
#endif
	return ret;
}

/**
 * @brief The database manager is initialized
 *
 * @param host
 * @param user
 * @param pswd
 * @param db_name
 * @return int
 */
int DbManager::initDb(std::string host, std::string user, std::string pswd, std::string db_name)
{
	// ReadMysqlInfo();
	conn = mysql_init(NULL);
	if (conn == NULL)
	{
#ifdef _D
		DBG(RED"<mysql> mysql_init fail\n" NONE);
#endif 
		return DB_CONN_INIT_FAIL;
	}
	conn = mysql_real_connect(conn, "127.0.0.1", "test", "123456", "ssp", 0, NULL, 0);
	// conn = mysql_real_connect(conn, host.c_str(), mysql_username, mysql_password, db_name.c_str(), 0, NULL, 0);
	if (conn == NULL)
	{
#ifdef _D
		DBG(RED"<mysql> mysql_real_connect fail\n" NONE);
#endif 
		return DB_CONN_CONNECT_FAIL;
	}
	return SUCCESS;
}

/**
 * @brief Execute a specific SQL statement
 *
 * @param sql
 * @return int
 * @tip abandon
 */
int DbManager::execSql(std::string sql)
{
	if (mysql_query(conn, sql.c_str()))
	{
		printf("[WARN    ]query fail : %s \n", mysql_error(conn));
		return DB_QUERY_FAIL;
	}
	else
	{
		result = mysql_use_result(conn);
		if (result)
		{
			int num_fields_1 = mysql_num_fields(result);
			int num_fields_2 = mysql_field_count(conn);
			int row_count = mysql_num_rows(result);
			printf("[DEBUG   ]row_num %d, fields_num %d, fields_num %d\n", row_count, num_fields_1, num_fields_2);
			while (result)
			{
				row = mysql_fetch_row(result);
				if (row == NULL)
				{
					printf("[DEBUG   ]empty row\n");
					break;
				}
				for (int j = 0; j < num_fields_1; j++)
				{
					printf("%s\t", row[j]);
				}
				printf("\n");
			}
			row_count = mysql_num_rows(result);
#ifdef _D
			DBG(GREEN "<mysql> row_num %d, fields_num %d, fields_num %d\n" NONE, row_count, num_fields_1, num_fields_2);
#endif
		}
		mysql_free_result(result);
	}

	return SUCCESS;
}

int DbManager::GetUserId()
{
	// select * from tb_var
	int used_user_id = 10000;
	int ret = mysql_query(conn, "select value from tb_var where key_str='user_id';");
	if (ret)
	{
		printf("[WARN    ]query user_id fail : %d %s \n", ret, mysql_error(conn));
		return used_user_id;
	}
	else
	{
		result = mysql_use_result(conn);
		if (result)
		{
			row = mysql_fetch_row(result);
			if (row == NULL)
			{
				printf("[DEBUG    ]query user_id empty\n");
			}
			sscanf(row[0], "%d", &used_user_id);
		}
		mysql_free_result(result);
	}
	return used_user_id;
}

int DbManager::GetUsersBegin()
{
	if (transection() == 1)
	{
		return DB_IS_BUSY;
	}
	set_transection(1); // Set to transaction
	int ret = mysql_query(conn, "select * from tb_user;");
	if (ret)
	{
#ifdef _D
		DBG(YELLOW"[mysql] query fail : %d %s \n" NONE, ret, mysql_error(conn));
#endif
		set_transection(0);
		return DB_QUERY_FAIL;
	}
	result = mysql_use_result(conn);
	return SUCCESS;
}

/**
 * @brief 
 * 
 * @param user An incoming outgoing parameter
 * @return int 
 */
int DbManager::GetUsersOneByOne(UserInfo *user)
{
	if (result)
	{
		row = mysql_fetch_row(result);
		if (row == NULL)
		{
#ifdef _D
			DBG(GREEN"<mysql> data acquisition completed\n" NONE);
#endif
			return DB_NO_MORE_DATA;
		}
		ssp::UserInfo pb_user;
		// 此处的10240指的是UserInfo字段
		pb_user.ParseFromArray(row[1], 10240);
#ifdef _D
		DBG(GREEN"<mysql> user_id: %d,user_name: %s\n" NONE, pb_user.user_id(), pb_user.user_name().c_str());
#endif
		user->set_user_id(pb_user.user_id());
		user->set_user_name(pb_user.user_name().c_str());
		user->set_nick_name(pb_user.nick_name().c_str());
		user->set_reg_time(pb_user.reg_time());
		user->set_from(pb_user.from());
		user->set_login_time(pb_user.login_time());
		user->set_last_login_time(pb_user.last_login_time());
		user->set_fresh_time(pb_user.fresh_time());
		user->set_password(pb_user.password().c_str());
		user->set_logout_time(pb_user.logout_time());	
	}
	else
	{
		return DB_NO_MORE_DATA;
	}
	return SUCCESS;
}

int DbManager::GetUsersEnd()
{
	mysql_free_result(result);
	set_transection(0);
	return SUCCESS;
}

int DbManager::InsertUser(UserInfo *user)
{
	/* 此处把数据转化成protobuf格式并存储到数据库 */
	ssp::UserInfo pb_user;

	pb_user.set_verion(1);
	pb_user.set_user_id(user->user_id());
	pb_user.set_user_name(user->user_name());
	pb_user.set_nick_name(user->nick_name());
	// pb_user.set_reg_time(user->reg_time());
	// pb_user.set_from(user->from());
	// pb_user.set_login_time(user->login_time());
	// pb_user.set_last_login_time(user->last_login_time());
	// pb_user.set_fresh_time(user->fresh_time());
	pb_user.set_password(user->password());
	// pb_user.set_logout_time(user->logout_time());

	char data[10240];
	pb_user.SerializeToArray(data, pb_user.ByteSize());
	char user_id[256];
	sprintf(user_id, "%d", user->user_id());
	std::string insertSql = "insert into tb_user values(";
	insertSql += user_id;
	insertSql += ",'";
	insertSql += data;
	insertSql += "');";
#ifdef _D
	DBG(BLUE"<mysql> insertSql:%s\n" NONE, insertSql.c_str());
#endif
	int ret = mysql_query(conn, insertSql.c_str());
	if (ret == 0)
	{
		return SUCCESS;
	}
	else
	{
#ifdef _D
		DBG(YELLOW"<mysql> insert fail : %d %s \n" NONE, ret, mysql_error(conn));
#endif
		return DB_QUERY_FAIL;
	}
	return SUCCESS;
}

int DbManager::UpdateUser(UserInfo *user)
{
	return SUCCESS;
}

int DbManager::DeleteUser(UserInfo *user)
{
	return SUCCESS;
}
