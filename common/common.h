/**
 * @file common.h
 * @author lh (2226958871@qq.com)
 * @brief The following are all the header files needed, including C library, 
 * C++STL library, C++ standard library, Linux network programming library, 
 * MySQL link library, Protobuf link library, custom macros and functions library, 
 * Windows network programming library
 * @version 0.1
 * @date 2022-02-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __COMMON_H__
#define __COMMON_H__

// my library
#include "color.h"
#include "macro.h"
#include "mode_value.h"
#include "mess_type.h"
#include "sock_error.h"
// uuid
#include "uuid.h"
// C standard library
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
// network programming
#ifdef ____Win32
#define STATE 0x01 // windows
#else
#define STATE 0x02 // linux
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/poll.h>
#include <sys/epoll.h>
#include <pthread.h>
#endif 
// proto
#include "../proto/UserInfo.pb.h"
#include "../proto/RelationInfo.pb.h"
#include "../proto/MessageInfo.pb.h"
#include "../proto/PhotoInfo.pb.h"
#include "../proto/MessType.pb.h"
// server
#include "../include/UserInfo.h"
#include "../include/RelationInfo.h"
#include "../include/MessageInfo.h"
#include "../include/PhotoInfo.h"
#include "../include/DbManager.h"
#include "../include/UserManager.h"
#include "../include/RelationManager.h"
#include "../include/MessageManager.h"
#include "../include/PhotoManager.h"
#include "../include/thread_pool.h"
// network
#include "../include/Socket.h"
#include "../include/MessType.h"
// db
// #include "mysql/mysql.h"
// cpp
#include <string>
#include <iostream>
// #include <unordered_map>

#ifdef _D  
#define DBG(fmt, args...) printf(fmt, ##args)
#else
#define DBG(fmt, args...)
#endif

#ifdef _P  
#define DATA(fmt, args...) printf(fmt, ##args)
#else
#define DATA(fmt, args...)
#endif

#ifdef _S
#define LOG(str) do \
{\
    fprintf(stderr, "[%s:%d %s %s]:%s\r\n",  __FILE__, __LINE__, __DATE__, __TIME__, str); \
}while(0)
#endif 

#endif // !__COMMON_H__