SRCS = $(wildcard ./src/*.cpp)
OBJS = $(SRCS:./src%.cpp=./bin%.o)
CFLAGS = -Wall -Wextra -Werror -std=c++20
NAME = 2dtest

run: $(NAME)
	@./$(NAME)

all: $(NAME)

./bin/%.o: ./src/%.cpp
	g++ -c $(CFLAGS) -I ./include $< -o $@

$(NAME): ${OBJS}
	g++ ${OBJS} -rpath /usr/local/lib -lSDL2main -lSDL2 -lSDL2_image -framework OpenGL -o $(NAME) 

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f $(NAME)

re: fclean all
