#include <iostream>

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

Channel::Channel(std::string name) : users_(), opers_(), name_(name) {}

Channel::Channel(void) : users_(), opers_() {}

Channel::~Channel(void) {}

bool	Channel::isUserAlreadyIn(STRING nick)
{
	MAP<STRING, Client*>::iterator client_iter = users_.find(nick);
	if (client_iter == users_.end())
		return (false);
	return (true);
}

void	Channel::assignUser(STRING nick, Client* new_user)
{
	// users_.insert(std::make_pair(nick, new_user));
    users_[nick] = new_user;
	// //"":nickName!userName@hostName JOIN #channName\n"
	// STRING	user_info = new_user->GetUserInfo();
	// STRING	proto_join = " JOIN " + name_ + "\r\n";
	// STRING proto_to_send;
	// proto_to_send = user_info + proto_join;
    
	
	// // 전체 멤버에게 join 명령어 보내기 + 이름 리스트
	// STRING name_list = "";
	// for (MAP<STRING, Client*>::iterator user_iter = users_.begin() ; 
    //     user_iter != users_.end() ;
	// 	++user_iter
	// )
	// {
	// 	Client * client_ptr = user_iter->second;
	// 	std::cout << "memeber: [" << client_ptr->GetNickName() << "] <- ";
	// 	std::cout << proto_to_send;
	// 	client_ptr->GetSendBuffer().append(proto_to_send);
		
	// 	STRING find_nick = client_ptr->GetNickName();
	// 	if (opers_.find(find_nick) != opers_.end())
	// 		name_list += "@";
	// 	name_list += client_ptr->GetNickName() + " ";
	// 	//name_list += client_ptr->getNickName() + "!" + client_ptr->getUserName() + "@" + client_ptr->getHostName() + " ";
	// }
	


}

void	Channel::assignOper(STRING nick, Client* user)
{
	opers_[nick] = user;
}


const std::string&	Channel::getName(void) const
{
	return name_;
}

MAP<STRING, Client*>& Channel::getUsers_()
{
	return users_;
}


MAP<STRING, Client*>& Channel::getOpers_()
{
	return opers_;
}

void Channel::eraseUser(STRING nick)
{
	MAP<STRING, Client*>::iterator iter = users_.find(nick);
	if (iter != users_.end())
		users_.erase(iter);
}

void Channel::eraseOper(STRING nick)
{
	MAP<STRING, Client*>::iterator iter = opers_.find(nick);
	if (iter != opers_.end())
		opers_.erase(iter);
}
