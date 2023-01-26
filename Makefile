NAME			=	ft_irc
CXX				=	clang++
CXXFLAGS		=	-Werror -Wall -Wextra -I$(INCL) -std=c++98 $(DEPEND_FLAGS)
INCL			=	./includes/
OBJSPATH		=	obj/
SRCSPATH		=	srcs/
SERVEROBJS		=	$(patsubst $(SRCSPATH)%, $(OBJSPATH)%, $(SRCS:.cpp=.o))
DEPS			=	$(SERVEROBJS:.o=.d)
DEBUG_FLAGS		=	-g3 -fstandalone-debug $(SANITIZE)
DEPEND_FLAGS	=	-MD
SANITIZE		=	-fsanitize=address

-include	sources.mk

all: $(NAME)

$(OBJSPATH)%.o:	$(SRCSPATH)%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(NAME):	$(SERVEROBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: LDFLAGS += $(SANITIZE)
debug: fclean $(NAME)

clean :
	rm -f *.o
	rm -rf obj
#		rm -f srcs/*.o
#		rm -f srcs/*.d

fclean : clean
	rm -f $(NAME)

re: fclean all

-include	$(DEPS)

.SILENT: clean fclean
.PHONY: clean fclean all re debug
