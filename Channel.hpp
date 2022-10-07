#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Define.hpp"

class Client;

class Channel
{
private:
	MAP<STRING , Client*>	users_;
	MAP<STRING , Client*>	opers_;

	STRING				topic_;
	STRING				name_; // tmp... considering to remove
	// Mode state;
public:
	Channel(std::string name); // todo: use reference?
	Channel(void);
	~Channel(void);
	// todo: OCCF

	void	assignUser(STRING , Client* new_user);
	void	assignOper(STRING , Client* user);

	bool	isUserAlreadyIn(STRING);

	const std::string&	getName(void) const;

	MAP<STRING, Client*>&	getUsers_();
	MAP<STRING, Client*>&	getOpers_();

	void eraseUser(STRING);
	void eraseOper(STRING);
};

#endif