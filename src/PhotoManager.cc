#include "../common/common.h"

void PhotoManager::Start()
{

}

void PhotoManager::Proc()
{

}

void PhotoManager::Shutdown()
{

}

void PhotoManager::Restart()
{
}

PhotoInfo *PhotoManager::GetPhoto(int user_id)
{
    for (int i = 0; i < photo_count_; i++)
    {
        if (photos_[i].user_id() == user_id)
        {
            return &photos_[i];
        }
    }
    return NULL;
}

int PhotoManager::UpdatePhoto(int user_id, int publisher_id, int publish_time, int publish_message_id)
{
    PhotoInfo *photo = GetPhoto(user_id);
    if (photo == NULL)
    {
        return PHOTO_NOT_EXIST;
    }
    photo->set_user_id(user_id); // need not
    photo->set_last_publisher(publisher_id);
    photo->set_last_publish_time(publish_time);
    photo->set_last_publish_message_id(publish_message_id);
    return SUCCESS;
};