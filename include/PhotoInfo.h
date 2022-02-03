#ifndef __PHOTO_INFO_H__

#define __PHOTO_INFO_H__

#include "../common/common.h"

class PhotoInfo
{
private:
    GETSETVAR(int, user_id)
    GETSETVAR(int, last_publisher)
    GETSETVAR(int, last_publish_time)
    GETSETVAR(int, last_publish_message_id)
};

#endif // !__PHOTO_INFO_H__