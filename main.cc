/**
 * @file main.cc
 * @author ssp (2226958871@qq.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "common/common.h"

DbManager db_svr;
UserManager user_svr;

int main(int argc, char **argv, char *env[]) {
    db_svr.Init();
    user_svr.Init(&db_svr);

    user_svr.Start();
    
#ifdef _D
    DBG(GREEN"Hello World!\n" NONE);
#endif // DEBUG
    return 0;
}