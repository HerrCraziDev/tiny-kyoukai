
PROJECT_NAME = tiny-kyoukai
BUILD_DIR = build

CC = gcc
CFLAGS = -std=c11 -g
LDFLAGS = -g
LIBS = gtk4 x11 gtk4-x11
LDLIBS = $(shell if [ -n "$(LIBS)" ]; then pkg-config -libs $(LIBS); fi)
INCLUDES = $(shell if [ -n "$(LIBS)" ]; then pkg-config -cflags $(LIBS); fi) -Isrc/includes

SRC = $(shell find src/ -type f -name '*.c') main.c
NOM = $(basename $(notdir $(SRC)))
OBJ = $(addprefix $(BUILD_DIR)/,$(addsuffix .o, $(NOM)))


#------------------------------------------------------------------------------
#								MAIN BUILD RULES
#------------------------------------------------------------------------------

# Build all the .o files in src dir
$(BUILD_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< $(INCLUDES) -o $(BUILD_DIR)/$(notdir $@)

# Build all the .o files in base dir
$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< $(INCLUDES) -o $(BUILD_DIR)/$(notdir $@)

# Main program
$(PROJECT_NAME): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Helloworld
kyoutest: 	$(BUILD_DIR)/helloworld.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@


#------------------------------------------------------------------------------
#								SILLY RULES
#------------------------------------------------------------------------------

always: SUSSYBAKA
	@echo "Aloha :3"

SUSSYBAKA:


#------------------------------------------------------------------------------
#								UTILITIES
#------------------------------------------------------------------------------

all: init build

build: $(PROJECT_NAME) localecho

rebuild: clean build

dev: build run

init: $(BUILD_DIR) clear

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

test:
	@echo CC: $(CC)
	@echo CFLAGS: $(CFLAGS)
	@echo LDFLAGS: $(LDFLAGS)
	@echo LDLIBS: $(LDLIBS)
	@echo OBJ: $(OBJ)
	@echo SRC: $(SRC)
	@echo INCLUDES: $(INCLUDES)

run: $(PROJECT_NAME)
	./$(PROJECT_NAME)

clean:
	@rm -rif *.o *.gch
	@rm -rif $(BUILD_DIR)/*.o $(BUILD_DIR)/*.gch

clear: clean
	@rm $(PROJECT_NAME)


.PHONY: always init SUSSYBAKA test clear clean