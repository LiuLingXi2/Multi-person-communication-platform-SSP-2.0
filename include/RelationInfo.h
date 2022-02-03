#ifndef __RELATION_INFO_H__

#define __RELATION_INFO_H__

#include "../common/common.h"

class RelationInfo
{
private:
	GETSETVAR(int, user_id);
	GETSETVAR(int, friend_count)
	int friend_list_[10240];
	GETSETVAR(int, black_count)
	int black_list_[10240];

public:
	int GetFriendUserIdByIndex(int index);
	int GetBlackUserIdByIndex(int index);
	int CheckFriend(int other_id);
	int CheckBlack(int other_id);
	int AddFriend(int other_id);
	int AddBlack(int other_id);
	int DeleteFriend(int other_id);
	int DeleteBlack(int other_id);
};
#endif // !__RELATION_INFO_H__