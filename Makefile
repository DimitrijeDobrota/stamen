NAME = menu
CC = g++

SRC = src
INC = include
BUILD = build

CFLAGS = -I$(INC) --std=c++17
LDFLAGS = 

SRCS := $(wildcard $(SRC)/*.cpp)
OBJS=$(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(SRCS))
TRGT=$(BUILD)/$(NAME)

ifeq ($(DEBUG),Y)
	CFLAGS += -ggdb -Wall -Wextra -Werror -Wpedantic -Weffc++
else
	CFLAGS += -Ofast -flto=auto
endif

all: dirs $(TRGT)
remake: clean
	$(MAKE)

$(TRGT): $(OBJS)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

$(BUILD)/%.o: $(SRC)/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS)

dirs:
	@mkdir -p $(BUILD)

clean:
	@$(RM) -rf $(OBJS) $(TRGT)

.PHONY: all clean remake dirs
