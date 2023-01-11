BASE_PATH		=	srcs/
BASE_SRCS		=	\
					main.cpp					\
					Server.cpp					\
					User.cpp					\
					Message.cpp					\
					cmd_nick.cpp				\
					cmd_user.cpp				\
					cmd_ping-pong.cpp			\
					cmd_quit.cpp				\
					cmd_privmsg.cpp				\
					utils.cpp					\
					cmd_pass.cpp

SRCS			=	$(addprefix $(BASE_PATH), $(BASE_SRCS))														

