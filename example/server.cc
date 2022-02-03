/**
 * @file server.cc
 * @author your name (you@domain.com)
 * @brief main
 * @version 0.1
 * @date 2022-02-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../common/common.h"

UserManager user_svr;
RelationManager rela_svr;
MessageManager mess_svr;
PhotoManager photo_svr;
DbManager db_svr;

int main()
{
    db_svr.Init();
    user_svr.Init(&db_svr);
    user_svr.Start();
    rela_svr.Start();
    mess_svr.Start();
    photo_svr.Start();

    int server_sock, clnt_sock;
    server_sock = SocketInit();
    clnt_sock = SocketAccept(clnt_sock, server_sock);

    char mess_type_str[10];
    unsigned char recv_buffer[10244] = {0}, send_buffer[10244] = {0};
    int server_on = 1, mess_type = -1, no_recv_count = 0, times = 0, ret;
    while (server_on)
    {
        int now = time(NULL);
        mess_type = SocketCheckRecv(clnt_sock, mess_type, recv_buffer, no_recv_count);
        if (mess_type < 0)
        {
            // usleep(5000);
            continue;
        }
        times++;
#ifdef _S
        DBG(GREEN "message count: %d\n" NONE, times);
#endif
        switch (mess_type)
        { /* Equilibrium model */
        case REG_REQ:
        {
            ssp::RegReq msgRegReq; // request
            ssp::RegRep msgRegRep; // reply
            /* [0]: version [1-3]: mess_type [4~]content*/
            msgRegReq.ParseFromArray(recv_buffer + 3, 10240);

            ret = user_svr.CreateUser(
                msgRegReq.user_name().c_str(),
                msgRegReq.password().c_str(),
                msgRegReq.from(), now);
            msgRegRep.set_ret(ret);
            if (ret == SUCCESS)
            {
                int user_id = user_svr.GetUserIdByUserName(msgRegReq.user_name().c_str());
                rela_svr.UserRelationInit(user_id);
                msgRegRep.set_user_id(user_id);
            }
            sprintf(mess_type_str, "%d", REG_REP);
            SetRspMessType(mess_type_str, send_buffer);
            ret = msgRegRep.SerializeToArray(send_buffer + 3, msgRegRep.ByteSize());
            SocketSendRep(msgRegRep.ByteSize() + 3, clnt_sock, send_buffer);

            break;
        }
        case LOGIN_REQ:
        {
            ssp::LoginReq msgLoginReq;
            ssp::LoginRep msgLoginRep;
            msgLoginReq.ParseFromArray(recv_buffer + 3, 10240);
            ret = user_svr.LoginCheck(msgLoginReq.user_name().c_str(),
                                      msgLoginReq.password().c_str());
            if (ret == SUCCESS)
            {
                int user_id = user_svr.GetUserIdByUserName(msgLoginReq.user_name().c_str());
                user_svr.UpdateUserLoginTime(user_id, now);
                msgLoginRep.set_user_id(user_id);
            }
            else
            {
                msgLoginRep.set_ret(ret);
                msgLoginRep.set_user_id(0);
            }
            msgLoginRep.set_ret(ret);
            sprintf(mess_type_str, "%d", LOGIN_REP);
            SetRspMessType(mess_type_str, send_buffer);
            ret = msgLoginRep.SerializeToArray(send_buffer + 3, msgLoginRep.ByteSize());
            SocketSendRep(msgLoginRep.ByteSize() + 3, clnt_sock, send_buffer);
            break;
        }
        case LOGOUT_REQ:
        {
            ssp::LogoutReq msgLogoutReq;
            ssp::LogoutRep msgLogoutRep;
            msgLogoutReq.ParseFromArray(recv_buffer + 3, 10240);
            ret = user_svr.CheckExist(msgLogoutReq.user_id());
            if (ret == USER_EXIST)
            {
                user_svr.UpdateUserLogoutTime(msgLogoutReq.user_id(), now);
                msgLogoutRep.set_ret(ret);
            }
            else
            {
                msgLogoutRep.set_ret(ret);
            }

            sprintf(mess_type_str, "%d", LOGOUT_REP);
            SetRspMessType(mess_type_str, send_buffer);
            msgLogoutRep.SerializeToArray(send_buffer + 3, msgLogoutRep.ByteSize());
            SocketSendRep(msgLogoutRep.ByteSize(), clnt_sock, send_buffer);
            break;
        }
        case ADD_FRIEND_REQ:
        {
            ssp::AddFriendReq msgAddFriendReq;
            ssp::AddFriendRep msgAddFriendRep;
            msgAddFriendReq.ParseFromArray(recv_buffer + 3, 10240);
            ret = user_svr.CheckExist(msgAddFriendReq.user_id());
            if (ret == USER_EXIST)
            {
                ret = user_svr.CheckExist(msgAddFriendReq.other_id());
                if (ret == USER_EXIST)
                {
                    ret = rela_svr.AddFriend(msgAddFriendReq.user_id(),
                                             msgAddFriendReq.other_id());
                    msgAddFriendRep.set_ret(ret);
                }
                msgAddFriendRep.set_ret(ret);
            }
            else
            {
                msgAddFriendRep.set_ret(ret);
            }
            sprintf(mess_type_str, "%d", ADD_FRIEND_REP);
            SetRspMessType(mess_type_str, send_buffer);
            ret = msgAddFriendRep.SerializeToArray(send_buffer + 3, msgAddFriendRep.ByteSize());
            SocketSendRep(msgAddFriendRep.ByteSize() + 3, clnt_sock, send_buffer);
            break;
        }
        case DEL_FRIEND_REQ:
        {
            ssp::DelFriendReq msgDelFriendReq;
            ssp::DelFriendRep msgDelFriendRep;
            msgDelFriendReq.ParseFromArray(recv_buffer + 3, 10240);
            ret = user_svr.CheckExist(msgDelFriendReq.user_id());
            if (ret == USER_EXIST)
            {
                ret = user_svr.CheckExist(msgDelFriendReq.other_id());
                if (ret == USER_EXIST)
                {
                    ret = rela_svr.DeleteFriend(msgDelFriendReq.user_id(),
                                                msgDelFriendReq.other_id());
                    msgDelFriendRep.set_ret(ret);
                }
                else
                {
                    msgDelFriendRep.set_ret(ret);
                }
            }
            else
            {
                msgDelFriendRep.set_ret(ret);
            }
            sprintf(mess_type_str, "%d", DEL_FRIEND_REP);
            SetRspMessType(mess_type_str, send_buffer);
            ret = msgDelFriendRep.SerializeToArray(send_buffer + 3, msgDelFriendRep.ByteSize());
            SocketSendRep(msgDelFriendRep.ByteSize() + 3, clnt_sock, send_buffer);
            break;
        }
        case ADD_BLACK_REQ:
        {
            ssp::AddBlackReq msgAddBlackReq;
            ssp::AddBlackRep msgAddBlackRep;
            msgAddBlackReq.ParseFromArray(recv_buffer + 3, 10240);
            ret = user_svr.CheckExist(msgAddBlackReq.user_id());
            if (ret == USER_EXIST)
            {
                ret = user_svr.CheckExist(msgAddBlackReq.other_id());
                if (ret == USER_EXIST)
                {
                    ret = rela_svr.AddBlack(msgAddBlackReq.user_id(),
                                            msgAddBlackReq.other_id());
                    msgAddBlackRep.set_ret(ret);
                }
                msgAddBlackRep.set_ret(ret);
            }
            else
            {
                msgAddBlackRep.set_ret(ret);
            }
            msgAddBlackRep.set_ret(ret);
            sprintf(mess_type_str, "%d", ADD_BLACK_REP);
            SetRspMessType(mess_type_str, send_buffer);
            msgAddBlackRep.SerializeToArray(send_buffer + 3, msgAddBlackRep.ByteSize());
            SocketSendRep(msgAddBlackRep.ByteSize(), clnt_sock, send_buffer);
            break;
        }
        case DEL_BLACK_REQ:
        {
            ssp::DelBlackReq msgDelBlackReq;
            ssp::DelBlackRep msgDelBlackRep;
            msgDelBlackReq.ParseFromArray(recv_buffer + 3, 10240);
            ret = user_svr.CheckExist(msgDelBlackReq.user_id());
            if (ret == USER_EXIST)
            {
                ret = user_svr.CheckExist(msgDelBlackReq.other_id());
                if (ret == USER_EXIST)
                {
                    ret = rela_svr.DeleteBlack(msgDelBlackReq.user_id(), msgDelBlackReq.other_id());
                    msgDelBlackRep.set_ret(ret);
                }
                else
                {
                    msgDelBlackRep.set_ret(ret);
                }
            }
            else
            {
                msgDelBlackRep.set_ret(ret);
            }

            sprintf(mess_type_str, "%d", DEL_BLACK_REP);
            SetRspMessType(mess_type_str, send_buffer);
            msgDelBlackRep.SerializeToArray(send_buffer + 3, msgDelBlackRep.ByteSize());
            SocketSendRep(msgDelBlackRep.ByteSize(), clnt_sock, send_buffer);
            break;
        }
        case PUBLISH_MESSAGE_REQ:
        {
            ssp::PublishMessageReq msgPublishMessageReq;
            ssp::PublishMessageRep msgPublishMessageRep;
            msgPublishMessageReq.ParseFromArray(recv_buffer + 3, 10240);
            ret = user_svr.CheckExist(msgPublishMessageReq.user_id());
            if (ret == USER_EXIST)
            {
                MessageInfo message;
                message.set_message_id(1); /* todo get from mysql */
                message.set_publisher(msgPublishMessageReq.user_id());
                message.set_publish_time(now);
                ret = mess_svr.PublishMessage(message);
                msgPublishMessageRep.set_ret(ret);
            }
            else
            {
                msgPublishMessageRep.set_ret(ret);
            }
            sprintf(mess_type_str, "%d", PUBLISH_MESSAGE_REP);
            SetRspMessType(mess_type_str, send_buffer);
            msgPublishMessageRep.SerializeToArray(send_buffer + 3, msgPublishMessageRep.ByteSize());
            SocketSendRep(msgPublishMessageRep.ByteSize() + 3, clnt_sock, send_buffer);
            break;
        }
        case GET_PHOTO_REQ:
        {
            ssp::GetPhotoReq msgGetPhotoReq;
            ssp::GetPhotoRep msgGetPhotoRep;
            msgGetPhotoReq.ParseFromArray(recv_buffer + 3, 10240);
            ret = user_svr.CheckExist(msgGetPhotoReq.user_id());
            if (ret == USER_EXIST)
            {
                PhotoInfo *photo = photo_svr.GetPhoto(msgGetPhotoReq.user_id());
                if (photo != NULL)
                {
                    MessageInfo *message = mess_svr.GetMessage(photo->last_publish_message_id());
                    if (message != NULL)
                    {
                        msgGetPhotoRep.mutable_last_message()->set_message_id(message->message_id());
                        msgGetPhotoRep.mutable_last_message()->set_content(message->content());
                        msgGetPhotoRep.mutable_last_message()->set_publisher(photo->last_publisher());
                        msgGetPhotoRep.mutable_last_message()->set_publish_time(photo->last_publish_time());
                        msgGetPhotoRep.set_ret(SUCCESS);
                    }
                    else
                    {
                        msgGetPhotoRep.set_ret(MESSAGE_NOT_EXIST);
                    }
                }
                else
                {
                    msgGetPhotoRep.set_ret(PHOTO_NOT_EXIST);
                }
            }
            else
            {
                msgGetPhotoRep.set_ret(ret);
            }
            sprintf(mess_type_str, "%d", GET_PHOTO_REP);
            SetRspMessType(mess_type_str, send_buffer);
            msgGetPhotoRep.SerializeToArray(send_buffer + 3, msgGetPhotoRep.ByteSize());
            SocketSendRep(msgGetPhotoRep.ByteSize() + 3, clnt_sock, send_buffer);
            break;
        }
        case GET_MESSAGE_LIST_REQ:
        {
            ssp::GetMessageReq msgGetMessageReq;
            ssp::GetMessageRep msgGetMessageRep;
            msgGetMessageReq.ParseFromArray(recv_buffer + 3, 10240);
            ret = user_svr.CheckExist(msgGetMessageReq.user_id());
            if (ret == USER_EXIST)
            {
                user_svr.UpdateUserFreshTime(msgGetMessageReq.user_id(), now);
                msgGetMessageRep.set_ret(SUCCESS);
            }
            else
            {
                msgGetMessageRep.set_ret(ret);
            }
            sprintf(mess_type_str, "%d", GET_MESSAGE_LIST_REP);
            SetRspMessType(mess_type_str, send_buffer);
            msgGetMessageRep.SerializeToArray(send_buffer + 3, msgGetMessageRep.ByteSize());
            SocketSendRep(msgGetMessageRep.ByteSize() + 3, clnt_sock, send_buffer);
            break;
        }
        default:
            server_on = 0;
#ifdef _D
            DBG(YELLOW "unknown mess_type: %d\n" NONE, mess_type);
#endif
            break;
        }
        user_svr.Proc();
        rela_svr.Proc();
        mess_svr.Proc();
        photo_svr.Proc();
        mess_type = -1;
        // usleep(5000);
    }
    SocketClose(server_sock);
    user_svr.Shutdown();
    rela_svr.Shutdown();
    mess_svr.Shutdown();
    photo_svr.Shutdown();

    return 0;
}