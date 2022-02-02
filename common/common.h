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
#endif 
// proto
#include "../proto/UserInfo.pb.h"
#include "../proto/RelationInfo.pb.h"
#include "../proto/MessageInfo.pb.h"
#include "../proto/PhotoInfo.pb.h"
// server
#include "../include/UserInfo.h"
#include "../include/RelationInfo.h"
#include "../include/MessageInfo.h"
#include "../include/PhotoInfo.h"
#include "../include/DbManager.h"
#include "../include/UserManager.h"
#include "../include/RelationManager.h"
#include "../include/MessageManager.h"
#include "../include/PhotoInfo.h"
// network
#include "../include/Socket.h"
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

#ifdef _S
#define LOG(str) do \
{\
    fprintf(stderr, "[%s:%d %s %s]:%s\r\n",  __FILE__, __LINE__, __DATE__, __TIME__, str); \
}while(0)
#endif 

#endif // !__COMMON_H__