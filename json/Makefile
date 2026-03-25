CC     = gcc
CFLAGS = -Wall -Wextra -Werror \
				 -std=c11 \
				 -Iinclude/
LFLAGS = 

BUILD_DIR  = build
SRC        = json test
SRC_O      = $(addsuffix .o,$(SRC))
SRC_O_PATH = $(addprefix $(BUILD_DIR)/,$(SRC_O))

test: $(SRC_O_PATH)
	$(CC) $(LFLAGS) -o $@.run $^

$(BUILD_DIR)/%.o: src/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ -c $^
