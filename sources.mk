BASE_PATH		=	srcs/
BASE_SRCS		=	\
					main.cpp					\
					Server.cpp					\
					User.cpp					\
					Message.cpp					\
					Channel.cpp					\
					utils.cpp

COMMAND_PATH	=	srcs/commands/
COMMAND_SRCS	=	\
					cmd_nick.cpp				\
					cmd_user.cpp				\
					cmd_ping-pong.cpp			\
					cmd_quit.cpp				\
					cmd_privmsg.cpp				\
					cmd_pass.cpp				\
					cmd_join.cpp				\
					cmd_list.cpp				\
					cmd_part.cpp				\
					cmd_kick.cpp				\
					cmd_oper.cpp				\
					cmd_names.cpp

SRCS			=	$(addprefix $(BASE_PATH), $(BASE_SRCS))			\
					$(addprefix $(COMMAND_PATH), $(COMMAND_SRCS))										

