CXX = c++
CXXFLAGS = -g -fsanitize=address
CXX_VERSION = -std=c++11
RM = rm -f

NAME = ircserv

SRC_FILE	=	main.cpp \
				Server.cpp \
				Client.cpp \
				ParseUtil.cpp \
				Command_user.cpp \
				Command_nick.cpp \
				Command_join.cpp \
				Channel.cpp
				
OBJ_FILE = $(SRC_FILE:.cpp=.o)
BONUS_FILE =

.PHONY: all
all: $(NAME)

$(NAME): $(OBJ_FILE)
	$(CXX) $(CXXFLAGS) $(CXX_VERSION) $^ -o $@ 

.PHONY: clean
clean:
	$(RM) $(OBJ_FILE)

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY: re
re:
	make fclean
	make all
