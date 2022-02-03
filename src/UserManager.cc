#include "../common/common.h"

/**
 * @brief Initialize the UserManager
 *
 * @param db_svr
 * @return int
 */
int UserManager::Init(DbManager *db_svr)
{
	db_svr_ = db_svr;
	set_user_count(0);
#ifdef _D
	DBG(GREEN "[%s %s USER  SERVER] UserManager Init\n" NONE, __DATE__, __TIME__);
#endif
	return SUCCESS;
}

/**
 * @brief Collect data from data into users_
 *
 * @return int
 */
int UserManager::Start()
{
	/* The ID is generated from the existing ID in the database each time */
	int used_user_id = 10000;
	set_cur_user_id(used_user_id);
	// Start a transaction and store all user table information in result
	int ret = db_svr_->GetUsersBegin();
	int ui = 0;
	if (ret == SUCCESS)
	{
		ret = db_svr_->GetUsersOneByOne(&users_[ui]);
		if (ret == SUCCESS)
		{
			ui++;
		}
		while (ret != DB_NO_MORE_DATA)
		{
			ret = db_svr_->GetUsersOneByOne(&users_[ui]);
			if (ret == SUCCESS)
				ui++;
		}
		ret = db_svr_->GetUsersEnd();
		set_user_count(ui);
	}
	else
	{
#ifdef _D
		DBG(RED "[%s %s USER   ERROR] UserManager Start fail\n" NONE, __DATE__, __TIME__);
#endif
		return ret;
	}
	ShowAll();
#ifdef _D
	DBG(GREEN "[%s %s USER  SERVER] UserManager Start\n" NONE, __DATE__, __TIME__);
#endif
	return SUCCESS;
}

/**
 * @brief UserManager Proc
 *
 * @return int
 */
int UserManager::Proc()
{
#ifdef _S
	DBG(GREEN "[%s %s USER  SERVER] UserManager Proc\n" NONE, __DATE__, __TIME__);
#endif
	return SUCCESS;
}

/**
 * @brief UserManager Shutdown
 *
 * @return int
 */
int UserManager::Shutdown()
{
	/* Write the data back */
	SaveUsers();
#ifdef _D
	DBG(GREEN "[%s %s USER  SERVER] UserManager Shutdown\n" NONE, __DATE__, __TIME__);
#endif
	return SUCCESS;
}

/**
 * @brief UserManager Restart
 *
 * @return int
 */
int UserManager::Restart()
{
#ifdef _D
	DBG(GREEN "[%s %s USER  SERVER] UserManager Restart\n" NONE, __DATE__, __TIME__);
#endif
	return SUCCESS;
}

/**
 * @brief Display all user information in dbg
 *
 * @return int
 */
int UserManager::ShowAll()
{
#ifdef _P
	DBG(GREEN "========================================\n" NONE);
	for (int i = 0; i < user_count(); i++)
	{
		DBG(GREEN "| %d | %s | %s |\n" NONE, users_[i].user_id(), users_[i].user_name(), users_[i].password());
	}
	DBG(GREEN "========================================\n" NONE);
#else
#endif
	return 0;
}

/**
 * @brief Obtain user information based on the user ID
 *
 * @param user_id
 * @return UserInfo*
 */
UserInfo *UserManager::GetUser(int user_id)
{
	for (int i = 0; i < user_count_; i++)
	{
		if (users_[i].user_id() == user_id)
		{
			return &users_[i];
		}
	}
	return NULL;
}

/**
 * @brief Check whether the user exists
 *
 * @param user_id
 * @return int
 */
int UserManager::CheckExist(int user_id)
{
	UserInfo *user = GetUser(user_id);
	if (user == NULL)
	{
#ifdef _D
		DBG(RED"[%s %s USER   ERROR] USER_NOT_EXIST %d\n", __DATE__, __TIME__, user_id);
#endif
		return USER_NOT_EXIST;
	}
	return USER_EXIST;
}

/**
 * @brief Create a user
 *
 * @param user_name
 * @param pswd
 * @param from
 * @param time_now
 * @return int
 */
int UserManager::CreateUser(const char *user_name, const char *pswd, int from, int time_now)
{
	for (int i = 0; i < user_count_; i++)
	{
		/* Set the rule that names cannot be the same */
		if (strcmp(users_[i].user_name(), user_name) == 0)
		{
#ifdef _D
			DBG(YELLOW "[%s %s USER WARNING] USER EXIST %s\n" NONE, __DATE__, __TIME__, users_[i].user_name());
#endif
			return USER_EXIST;
		}
	}

	if (user_count_ < ONLINE_NUM)
	{
		/* The server assigns a user_id to the user (UUID) */
		users_[user_count_].set_user_id(cur_user_id()); // todo
		users_[user_count_].set_user_name(user_name);
		// users_[user_count_].set_nick_name(user_name);
		// users_[user_count_].set_reg_time(time_now);
		// users_[user_count_].set_from(from);
		// users_[user_count_].set_login_time(time_now);
		// users_[user_count_].set_last_login_time(time_now);
		// users_[user_count_].set_fresh_time(time_now);
		users_[user_count_].set_password(pswd);
		// users_[user_count_].set_logout_time(0); // todo
		db_svr_->InsertUser(&users_[user_count_]);
		/* Identifying user status */
		users_[user_count_].set_db_flag(FLAG_INSERT);
		user_count_++;
	}
	set_reg_num(reg_num() + 1);
	set_cur_user_id(cur_user_id() + 1);

	/* Write back to the database */
	// SaveUsers();
	// users_[user_count_].set_db_flag(FLAG_INSERT);

	return 0;
}

/**
 * @brief The cancellation of the user
 *
 * @param user_id
 * @return int
 */
int UserManager::DeleteUser(int user_id)
{
	for (int i = 0; i < user_count_; i++)
	{
		/* Because the user already exists, the user user_id is used to verify this */
		if (users_[i].user_id() == user_id)
		{
#ifdef _D
			DBG("YELLOW[%s %s USER WARNING] USER EXIST %d %s\n", __DATE__, __TIME__, users_[i].user_id(), users_[i].user_name());
#endif
			users_[i].set_db_flag(FLAG_DELETE);
			return USER_EXIST;
		}
	}

	SaveUsers(); // todo
	return 0;
}

/**
 * @brief Update the database status of the user based on the db_flag identifier
 *
 * @return int
 */
int UserManager::SaveUsers()
{
	for (int i = 0; i < user_count_; i++)
	{
		/* 1 delete */
		if (users_[i].db_flag() == FLAG_DELETE)
		{
			db_svr_->DeleteUser(&users_[i]);
			users_[i].set_db_flag(FLAG_INIT);
		}
		/* 2 update */
		if (users_[i].db_flag() == FLAG_UPDATE)
		{
			db_svr_->UpdateUser(&users_[i]);
			users_[i].set_db_flag(FLAG_INIT);
		}
		/* 3 insert */
		if (users_[i].db_flag() == FLAG_INSERT)
		{
			db_svr_->InsertUser(&users_[i]);
			users_[i].set_db_flag(FLAG_INIT);
		}
	}
	return 0;
}

/**
 * @brief Check whether the user is logged in
 *
 * @param user_name
 * @param user_pswd
 * @return int
 */
int UserManager::LoginCheck(const char *user_name, const char *user_pswd)
{
	for (int i = 0; i < user_count_; i++)
	{
		if (strcmp(users_[i].user_name(), user_name) == 0)
		{
			if (strcmp(users_[i].password(), user_pswd) == 0)
			{
				return SUCCESS;
			}
			else
			{
				return WRONG_PASSWORD;
			}
		}
	}
	return USER_NOT_EXIST;
}

/**
 * @brief Updated the user logout time
 *
 * @param user_id
 * @param now
 * @return int
 */
int UserManager::UpdateUserLogoutTime(int user_id, int time_now)
{
	UserInfo *user = GetUser(user_id);
	if (user == NULL)
	{
		return USER_NOT_EXIST;
	}
	user->set_logout_time(time_now);
	user->set_db_flag(FLAG_UPDATE);
	return SUCCESS;
}

/**
 * @brief Obtain the user ID based on the user name
 *
 * @param user_name
 * @return int
 */
int UserManager::GetUserIdByUserName(const char *user_name)
{
	for (int i = 0; i < user_count_; i++)
	{
		if (strcmp(users_[i].user_name(), user_name) == 0)
		{
			return users_[i].user_id();
		}
	}
	return USER_NOT_EXIST;
}

/**
 * @brief Update login time
 *
 * @param user_id
 * @param time_now
 * @return int
 */
int UserManager::UpdateUserLoginTime(int user_id, int time_now)
{
	UserInfo *user = GetUser(user_id);
	if (user == NULL)
	{
#ifdef _D
		DBG("[%s %s USER   ERROR] USER_NOT_EXIST %d\n", __DATE__, __TIME__, user_id);
#endif
		return USER_NOT_EXIST;
	}
	user->set_login_time(time_now);
	user->set_db_flag(FLAG_UPDATE);
	return SUCCESS;
}

/**
 * @brief Update refresh time
 *
 * @param user_id
 * @param time_now
 * @return int
 */
int UserManager::UpdateUserFreshTime(int user_id, int time_now)
{
	UserInfo *user = GetUser(user_id);
	if (user == NULL)
	{
#ifdef _D
		DBG("[%s %s USER   ERROR] USER_NOT_EXIST %d\n", __DATE__, __TIME__, user_id);
#endif
		return USER_NOT_EXIST;
	}
	user->set_fresh_time(time_now);
	user->set_db_flag(FLAG_UPDATE);
	return SUCCESS;
}