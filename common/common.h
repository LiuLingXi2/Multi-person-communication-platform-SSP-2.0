#ifndef __COMMON_H__
#define __COMMON_H__

// my library
#include "color.h"
#include "macro.h"
#include "ret_value.h"
// C standard library
#include <stdio.h>
#include <string.h>
#include <stdint.h>
// network programming
#ifdef ____Win32
#define STATE 0x01 // windows
#else
#define STATE 0x02 // linux
#include <unistd.h>
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
// db
// #include "mysql/mysql.h"
// cpp
#include <string>
#include <iostream>
#include <unordered_map>
// uuid
#include "uuid.h"

#ifdef _D   
#define DBG(fmt, args...) printf(fmt, ##args)
#else
#define DBG(fmt, args...)
#endif

#endif // !__COMMON_H__