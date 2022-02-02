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

#include "include/head.h"

int main(int argc, char **argv, char *env[]) {
    
#ifdef DEBUG
    DBG(YELLOW"Hello World!\n" NONE);
#endif // DEBUG
    return 0;
}