CC = g++
CFLAGS = -Wall -std=c++20
LDFLAGS = 
BUILD_DIR = ./build
RELEASE_DIR = ./release

# Source files
SERVER_SRC = server.cpp
CLIENT_SRC = client.cpp
THREAD_RACER_SRC = thread_racer.cpp

# Object files
SERVER_OBJ = $(BUILD_DIR)/server.o
CLIENT_OBJ = $(BUILD_DIR)/client.o
THREAD_RACER_OBJ = $(BUILD_DIR)/thread_racer.o

# Executables
SERVER = $(RELEASE_DIR)/server
CLIENT = $(RELEASE_DIR)/client
THREAD_RACER = $(RELEASE_DIR)/thread_racer

.PHONY: all clean

all: $(BUILD_DIR) $(RELEASE_DIR) $(SERVER) $(CLIENT) $(THREAD_RACER)

$(SERVER): $(SERVER_OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(CLIENT): $(CLIENT_OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(THREAD_RACER): $(THREAD_RACER_OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(RELEASE_DIR):
	mkdir -p $(RELEASE_DIR)

clean:
	rm -rf $(BUILD_DIR) $(RELEASE_DIR)