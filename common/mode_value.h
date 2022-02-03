#ifndef __RET_VALUE_H__

#define __RET_VALUE_H__

#define VERSION 1

#define ONLINE_NUM 10239 /* Number of concurrent online users */

#define OK 0
#define ERROR -1
#define SUCCESS 0

#define USER_EXIST -100
#define USER_NOT_EXIST -101
#define USER_NEED_DELETE -102
#define WRONG_PASSWORD -103

#define NOT_FRIEND -200
#define ALREADY_FRIEND -201
#define FRIEND_TO_MUCH -202
#define FRIEND_NEED_DELETE -203

#define RELATION_NOT_FOUND -204

#define NOT_BLACK -300
#define ALREADY_BLACK -301
#define BLACK_TO_MUCH -302
#define BLACK_NEED_DELETE -303

#define MESSAGE_NOT_EXIST -400

#define PHOTO_NOT_EXIST -500

#define DB_CONN_INIT_FAIL -600
#define DB_CONN_CONNECT_FAIL -601
#define DB_QUERY_FAIL -602
#define DB_IS_BUSY -603
#define DB_NO_MORE_DATA -604


#define FLAG_INIT 0
#define FLAG_DELETE 1
#define FLAG_UPDATE 2
#define FLAG_INSERT 3

#endif // !__RET_VALUE_H__