NAME = my_trojan

CC = clang
CFLAGS =	-g -Wall -Wextra -Werror \
			-std=c99 \
			-Wpedantic \
			-Wconversion \
			-Wdouble-promotion \
			-Wno-unused-parameter \
			-Wno-unused-function \
			-Wno-sign-conversion \
			-fsanitize=undefined \
			-fsanitize-trap

# payload
PAYLOAD_OUT = payload
PAYLOAD_SRC_DIR = src/payload
PAYLOAD_OBJ_DIR = obj/payload

PAYLOAD_SRCS = $(wildcard $(PAYLOAD_SRC_DIR)/*.c)
PAYLOAD_OBJS = $(patsubst $(PAYLOAD_SRC_DIR)/%.c, $(PAYLOAD_OBJ_DIR)/%.o, $(PAYLOAD_SRCS))

# trojan
TROJAN_SRC_DIR = src/trojan
TROJAN_OBJ_DIR = obj/trojan

TROJAN_SRCS =	$(TROJAN_SRC_DIR)/main.c \
				$(TROJAN_SRC_DIR)/payload.c
TROJAN_OBJS = $(patsubst $(TROJAN_SRC_DIR)/%.c, $(TROJAN_OBJ_DIR)/%.o, $(TROJAN_SRCS))

.PHONY: all
all: help

.PHONY: help
help: ## Prints help for targets with comments
	@echo "Available Rules:"
	@cat $(MAKEFILE_LIST) | grep -E '^[a-zA-Z_-]+:.*?## .*$$' | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

# directories
$(PAYLOAD_OBJ_DIR):
	@mkdir -p $(PAYLOAD_OBJ_DIR)

$(TROJAN_OBJ_DIR):
	@mkdir -p $(TROJAN_OBJ_DIR)

# compile payload sources
$(PAYLOAD_OBJ_DIR)/%.o: $(PAYLOAD_SRC_DIR)/%.c | $(PAYLOAD_OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# compile trojan sources
$(TROJAN_OBJ_DIR)/%.o: $(TROJAN_SRC_DIR)/%.c | $(TROJAN_OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# link payload executable
$(PAYLOAD_OUT): $(PAYLOAD_OBJS)
	@$(CC) $(CFLAGS) $^ -o $@

# generate payload C array via xxd
$(TROJAN_SRC_DIR)/payload.c: $(PAYLOAD_OUT)
	@xxd -i $< > $@

# link my_trojan executable
$(NAME): $(TROJAN_OBJS)
	@$(CC) $(CFLAGS) $^ -o $@

.PHONY: build
build: $(NAME) ## Builds the binary

.PHONY: clean
clean: ## Cleans transitive dependencies
	@rm -rf $(PAYLOAD_OBJ_DIR) $(TROJAN_OBJ_DIR) $(PAYLOAD_OUT)

.PHONY: fclean
fclean: clean ## Cleans transitive dependencies and the binary
	@rm -f $(NAME) $(TROJAN_SRC_DIR)/payload.c

.PHONY: re
re: fclean build ## Rebuilds the binary

.PHONY: t
t: $(NAME) ## Runs the binary for test
	@./$(NAME)
