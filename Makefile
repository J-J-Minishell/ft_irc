NAME		=	ft_irc
CXX			=	clang++
CXXFLAGS	=	-Werror -Wall -Wextra -I$(INCL) -std=c++98
INCL		=	./includes/
OBJSPATH	=	obj/
SRCSPATH	=	srcs/
SERVEROBJS	=	$(patsubst $(SRCSPATH)%, $(OBJSPATH)%, $(SRCS:.cpp=.o))
DEBUG_FLAGS	=	-g3 -fstandalone-debug

-include	sources.mk

all: $(NAME)

$(OBJSPATH)%.o:	$(SRCSPATH)%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(NAME):	$(SERVEROBJS)
	$(CXX) -o $@ $^

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(NAME)

clean :
	rm -f *.o
		rm -f srcs/*.o
		rm -f srcs/*.d
	rm -rf obj

fclean : clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean all re debug