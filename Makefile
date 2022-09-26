CXX = c++
CXX_VERSION = -std=c++11
RM = rm -f

NAME = ircserv

SRC_FILE	=	main.cpp \
				server.cpp \
				
OBJ_FILE = $(SRC_FILE:.cpp=.o)
BONUS_FILE =

.PHONY: all
all: $(NAME)

$(NAME): $(OBJ_FILE)
	$(CXX) $(CXX_VERSION) $^ -o $@ 

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
