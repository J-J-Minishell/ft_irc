BASE_PATH		=	srcs/
BASE_SRCS		=	\
					main.cpp					\
					Server.cpp					\
					User.cpp					\
					Message.cpp					\
					nick_cmd.cpp				\
					user_cmd.cpp				\
					utils.cpp

SRCS			=	$(addprefix $(BASE_PATH), $(BASE_SRCS))														

