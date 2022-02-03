#include "../common/common.h"

int UserManager::Init(DbManager *db_svr)
{
    db_svr_ = db_svr;
#ifdef _D
    DBG(GREEN "<user> UserManager Init\n" NONE);
#endif
    return SUCCESS;
}

int UserManager::Start()
{
    // Start a transaction and store all user table information in result
	int ret = db_svr_->GetUsersBegin(); 
	int ui = 0;
	if (ret == SUCCESS)
	{
		ret = db_svr_->GetUsersOneByOne(&users_[ui]);
		if (ret == SUCCESS) {
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
		DBG(RED"<user> UserManager Start fail\n" NONE);
#endif
		return ret;
	}
	ShowAll();
#ifdef _D
	DBG(GREEN"UserManager Start\n" NONE);
#endif
	return SUCCESS;
}

int UserManager::Proc()
{
#ifdef _D
    DBG(GREEN"<user> UserManager Proc\n" NONE);
#endif
    return SUCCESS;
}

int UserManager::Shutdown()
{
    /* Write the data back */
    SaveUsers();
#ifdef _D
    DBG(GREEN "<user> UserManager Shutdown\n" NONE);
#endif
    return SUCCESS;
}

int UserManager::Restart()
{
#ifdef _D
    DBG(GREEN "<user> UserManager Restart\n" NONE);
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
#ifdef _D
	DBG(GREEN"========================================\n" NONE);
	for (int i = 0; i < user_count(); i++)
	{
		DBG(GREEN"| %d | %s | %s |\n" NONE, users_[i].user_id(), users_[i].user_name(), users_[i].nick_name());
	}
	DBG(GREEN"========================================\n" NONE);
#else
#endif
	return 0;
}

UserInfo *UserManager::GetUser(int user_id)
{
	return SUCCESS;
}

int UserManager::CheckExist(int user_id)
{
	return SUCCESS;
}

int UserManager::CreateUser(const char *user_name, const char *pswd, int from, int time_now)
{
	for (int i = 0; i < user_count_; i++)
	{
		if (strcmp(users_[i].user_name(), user_name) == 0)
		{
#ifdef _D
			DBG(YELLOW"USER EXIST %d %s\n" NONE, users_[i].user_id(), users_[i].user_name());
#endif
			return USER_EXIST;
		}
	}

	if (user_count_ < 10239)
	{
		users_[user_count_].set_user_id(2); // todo
		users_[user_count_].set_user_name(user_name);
		users_[user_count_].set_nick_name(user_name);
		// users_[user_count_].set_reg_time(time_now);
		// users_[user_count_].set_from(from);
		// users_[user_count_].set_login_time(time_now);
        // users_[user_count_].set_last_login_time(time_now);
        // users_[user_count_].set_fresh_time(time_now);
        users_[user_count_].set_password(pswd);
        // users_[user_count_].set_logout_time(0); // todo

		users_[user_count_].set_db_flag(FLAG_INSERT);
		user_count_++;
	}
	set_reg_num(reg_num() + 1);

	SaveUsers();

	return 0;
}

int UserManager::DeleteUser(int user_id)
{
	return SUCCESS;
}

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

int UserManager::LoginCheck(const char *user_name, const char *user_pswd)
{
	return SUCCESS;
}

int UserManager::UserLogout(int user_id, int now)
{
	return SUCCESS;
}

int UserManager::GetUserIdByUserName(const char *user_name)
{
	return SUCCESS;
}

int UserManager::UpdateUserLoginTime(int user_id, int time_now)
{
	return SUCCESS;
}

int UserManager::UpdateUserFreshTime(int user_id, int time_now)
{
	return SUCCESS;
}
