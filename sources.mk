BASE_PATH		=	srcs/
BASE_SRCS		=	\
					main.cpp					\
					Server.cpp					\
					User.cpp					\
					Message.cpp					\
					cmd_nick.cpp				\
					cmd_user.cpp				\
					utils.cpp

SRCS			=	$(addprefix $(BASE_PATH), $(BASE_SRCS))														

