# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alienard <alienard@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/09 18:51:33 by alienard          #+#    #+#              #
#    Updated: 2022/06/14 16:46:17 by alienard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 		miniRT

OS_NAME := $(shell uname -s)

MLX_DEF = -D $(OS_NAME)

MLX = libmlx.a
MLX_LIBS = -lmlx
MLX_INC = -L$(MLX_DIR)

MLX_DIR =
ifeq ($(OS_NAME),Darwin)
	MLX_DIR += mlx
	MLX_INC += -framework OpenGL -framework AppKit
else
	MLX_DIR += mlx_linux
	MLX_LIBS += -lXext -lX11 -lm -lz
	KB_LAYOUT += $(shell setxkbmap -query | grep -oP "layout:\s+\K\w+")
endif

LIB = libft.a
LIB_INC = -L$(LIBPATH) -lft
LIBPATH = libft

INC = -I includes -I$(LIBPATH) -I$(MLX_DIR)

SRC_DIR = src

SRC = $(addprefix $(SRC_DIR)/,$(SRC_NAME))

SRC_NAME =		ft_mini_rt.c \
			ft_parsing.c \
			ft_parsing_2.c \
			ft_bzero_struct.c \
			ft_check_parsing.c \
			ft_check_parsing_2.c \
			ft_cam.c \
			ft_light.c \
			ft_sphere.c \
			ft_plane.c \
			ft_square.c \
			ft_cylinder.c \
			ft_cylinder_2.c \
			ft_triangle.c \
			ft_argb.c \
			ft_event.c \
			ft_vectors.c \
			ft_vectors_2.c \
			ft_scalar.c \
			ft_matrix.c \
			ft_ray.c \
			ft_ray_2.c \
			ft_pt.c \
			ft_close.c \
			ft_save.c \
			ft_error.c

OBJ_DIR = obj

OBJ_NAME = $(SRC_NAME:.c=.o)

OBJ = $(addprefix $(OBJ_DIR)/,$(OBJ_NAME))

CC =		gcc

RM = 		rm -f

CFLAGS =	-Wall -Wextra -Werror -g3 -ofast

SANFLAGS =	-Wall -Wextra -Werror -g3 -fsanitize=address -fno-omit-frame-pointer

all:		$(LIB) $(MLX) $(NAME)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	    $(CC) $(INC) $(CFLAGS) $(MLX_DEF) -c $< -o $@

$(OBJ_DIR):
		mkdir -p $@


$(NAME):	$(OBJ)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(INC) $(LIB_INC) $(MLX_INC) $(MLX_LIBS)

$(MLX):
		@make -C $(MLX_DIR) $(MLX_LIBS)

$(LIB):
		@make -C $(LIBPATH)

clean:
			$(RM) -r $(OBJ_DIR)
			@make -C $(LIBPATH) clean

fclean:		clean
			$(RM) $(NAME)
			@make -C $(MLX_DIR) clean
			@make -C $(LIBPATH) fclean

re:			fclean all

sanitize:	$(MLX)
			@make -C $(LIBPATH) fclean
			@make -C $(LIBPATH)
			$(RM) -r $(OBJ_DIR)
			$(RM) $(NAME)
			$(MAKE) CFLAGS="$(SANFLAGS)" $(NAME)

.PHONY:		re all clean fclean sanitize
