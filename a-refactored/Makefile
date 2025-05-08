# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shinckel <shinckel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/26 21:27:04 by shinckel          #+#    #+#              #
#    Updated: 2025/05/08 18:17:43 by shinckel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

# Source files
SRCS =	src/main.cpp \
				src/core/Socket.cpp \
				src/core/Server.cpp \
        src/core/Client.cpp \
        src/core/Channel.cpp \
				src/commands/CapCommand.cpp \
				src/commands/CommandDispatcher.cpp \
				src/commands/InviteCommand.cpp \
        src/commands/JoinCommand.cpp \
        src/commands/KickCommand.cpp \
        src/commands/TopicCommand.cpp \
        src/commands/NickCommand.cpp \
        src/commands/UserCommand.cpp \
        src/commands/ModeCommand.cpp \
				src/commands/PassCommand.cpp \
				src/commands/PrivmsgCommand.cpp \
				src/commands/QuitCommand.cpp \
				src/commands/WhoCommand.cpp \
        src/utils/Parser.cpp \
        src/utils/Error.cpp

# Compiler and flags
CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -Iinclude

# Define the directory for object files
OBJDIR = objs

# Object files
OBJS = $(SRCS:src/%.cpp=$(OBJDIR)/%.o)

# Define colors
RED = \033[1;41m
GREEN = \033[1;42m
RESET = \033[0m

# Default rule
all: $(NAME)

# Rule to compile .cpp files to .o files in the OBJDIR directory
$(OBJDIR)/%.o: src/%.cpp
			@mkdir -p $(dir $@)  # Create the OBJDIR directory and subdirectories if they don't exist
			@echo "Compiling $<..."
			$(CXX) $(CXXFLAGS) -c $< -o $@

# Link the object files to create the final executable
$(NAME): $(OBJS)
			@echo "Linking $(NAME)..."
			$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
			@echo "$(GREEN) Ready to CHAT!!! $(RESET)"

# Clean object files
clean:
		@rm -f $(OBJS)
		@echo "$(RED) Remove object files $(RESET)"

# Clean everything, including the executable
fclean: clean
		@rm -f $(NAME)
		@rm -rf $(OBJDIR)
		@echo "$(RED) Remove $(NAME) $(RESET)"

# Rebuild the project
re: fclean all

.PHONY: all clean fclean re