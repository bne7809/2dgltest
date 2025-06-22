SRCS = $(wildcard ./src/*.cpp)
OBJS = $(SRCS:./src%.cpp=./bin%.o)
CFLAGS = -Wall -Wextra -Werror
NAME = 2dtest

run: $(NAME)
	@./$(NAME)

all: $(NAME)

./bin/%.o: ./src/%.cpp | bin
	clang++ -c $(CFLAGS) -fno-exceptions -std=c++20 -DGL_SILENCE_DEPRECATION -I ./include $< -o $@

./bin/glad.o: ./src/glad.c
	clang -c $(CFLAGS) $< -o $@

$(NAME): ${OBJS} ./bin/glad.o
	clang++ ${OBJS} ./bin/glad.o -lglfw.3 -framework OpenGL -o $(NAME) 
	install_name_tool -add_rpath /usr/local/lib ./$(NAME)

clean:
	rm -f ${OBJS} ./bin/glad.o

fclean: clean
	rm -f $(NAME)

bin:
	mkdir -p bin

re: fclean all
