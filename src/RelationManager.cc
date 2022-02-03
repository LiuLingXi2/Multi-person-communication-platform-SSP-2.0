#include "../common/common.h"

void RelationManager::Start(){
#ifdef _D
	DBG(GREEN"[%s %s RELA   SERVER] RelationManager Start\n" NONE, __DATE__, __TIME__);
#endif
}
void RelationManager::Proc(){
#ifdef _D
	DBG(GREEN"[%s %s RELA   SERVER] RelationManager Proc\n" NONE, __DATE__, __TIME__);
#endif
}

void RelationManager::Shutdown(){
#ifdef _D
	DBG(GREEN"[%s %s RELA   SERVER] RelationManager Shutdown\n" NONE, __DATE__, __TIME__);
#endif
}

void RelationManager::Restart(){
#ifdef _D
	DBG(GREEN"[%s %s RELA   SERVER] RelationManager Restart\n" NONE, __DATE__, __TIME__);
#endif
}

int RelationManager::UserRelationInit(int user_id){
	relations_[relation_count_].set_user_id(user_id);
	set_relation_count(relation_count()+1);
}
RelationInfo* RelationManager::GetRelation(int user_id){
	for(int i=0;i<relation_count_;i++){
		if(relations_[i].user_id()==user_id){
			return &relations_[i];
		}
	}
	return NULL;
}

int RelationManager::AddFriend(int user_id,int other_id){
	RelationInfo* relation=GetRelation(user_id);
	if(relation==NULL){
		return RELATION_NOT_FOUND;
	}
	int ret=relation->AddFriend(other_id);
	return ret;
}

int RelationManager::DeleteFriend(int user_id,int other_id){
	RelationInfo* relation=GetRelation(user_id);
	if(relation==NULL){
		return RELATION_NOT_FOUND;
	}
	int ret=relation->DeleteFriend(other_id);
	return ret;
}

int RelationManager::AddBlack(int user_id,int other_id){
	RelationInfo* relation=GetRelation(user_id);
	if(relation==NULL){
		return RELATION_NOT_FOUND;
	}
	int ret=relation->AddBlack(other_id);
	return ret;
}

int RelationManager::DeleteBlack(int user_id,int other_id){
	RelationInfo* relation=GetRelation(user_id);
        if(relation==NULL){
                return RELATION_NOT_FOUND;
        }
        int ret=relation->DeleteBlack(other_id);
        return ret;
}