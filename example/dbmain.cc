/**
 * @file dbmain.cc
 * @author lh (2226958871@qq.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../common/common.h"

int main(){
	DbManager db_svr;
	db_svr.initDb("127.0.0.1","hank","12341234","ssp");
	db_svr.execSql("truncate tb_user;");
	return 0;
}