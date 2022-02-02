#include "../common/common.h"

class RelationInfo {
private:
	GETSETVAR(int,user_id);
	GETSETVAR(int,friend_count)
	int friend_list_[10240];
	GETSETVAR(int,black_count)
	int black_list_[10240];
};