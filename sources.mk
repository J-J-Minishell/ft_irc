BASE_PATH		=	srcs/
BASE_SRCS		=	\
					main.cpp					\
					Server.cpp					\
					User.cpp					\
					Message.cpp					\
					Channel.cpp					\
					cmd_nick.cpp				\
					cmd_user.cpp				\
					cmd_ping-pong.cpp			\
					cmd_quit.cpp				\
					cmd_privmsg.cpp				\
					cmd_pass.cpp				\
					cmd_join.cpp				\
					utils.cpp					

SRCS			=	$(addprefix $(BASE_PATH), $(BASE_SRCS))														

