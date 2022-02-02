#ifndef __RET_VALUE_H__

#define __RET_VALUE_H__

#define VERSION 1

#define SUCCESS 0
// db
#define DB_CONN_INIT_FAIL -101
#define DB_CONN_CONNECT_FAIL -102
#define DB_QUERY_FAIL -103
#define DB_NO_MORE_DATA -104
#define DB_IS_BUSY -105
// user
#define USER_EXIST -200

// flag
#define FLAG_INIT   0x01
#define FLAG_INSERT 0x02
#define FLAG_DELETE 0x03
#define FLAG_UPDATE 0x04

#endif // !__RET_VALUE_H__