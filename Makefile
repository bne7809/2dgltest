BUILD ?= debug

SRCS = $(wildcard ./src/*.cpp)
OBJS = $(SRCS:./src%.cpp=./bin%.o)
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -fno-exceptions -fno-rtti -fvisibility=hidden -ffunction-sections -fdata-sections 
NAME = 2dtest

ifeq ($(BUILD),debug)
	CFLAGS += -g -O0
	CPPOPT = 
else ifeq ($(BUILD),release)
	CFLAGS += -DNDEBUG
	CPPOPT = -O2 -flto
else
	$(error Invalid BUILD type.)
endif

all: $(NAME)

run: $(NAME)
	@./$(NAME)

./bin/%.o: ./src/%.cpp | bin
	clang++ -c $(CFLAGS) $(CPPFLAGS) $(CPPOPT) -std=c++20 -DGL_SILENCE_DEPRECATION -I ./include $< -o $@

./bin/glad.o: ./src/glad.c
	clang -c $(CFLAGS) $< -o $@

$(NAME): ${OBJS} ./bin/glad.o
	clang++ $(CPPOPT) -Wl,-dead_strip ${OBJS} ./bin/glad.o -lglfw.3 -framework OpenGL -o $(NAME) 
	install_name_tool -add_rpath /usr/local/lib ./$(NAME)
ifeq ($(BUILD),release)
	strip $(NAME)
endif

clean:
	rm -f ${OBJS} ./bin/glad.o

fclean: clean
	rm -f $(NAME)

bin:
	mkdir -p bin

re: fclean all

.PHONY: all clean fclean re run
