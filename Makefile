CC = g++
CFLAGS = -Wall -std=c++20
LDFLAGS = 

# Source files
SERVER_SRC = server.cpp
CLIENT_SRC = client.cpp
RACER_SRC = thread_racer.cpp

# Object files
SERVER_OBJ = $(SERVER_SRC:.cpp=.o)
CLIENT_OBJ = $(CLIENT_SRC:.cpp=.o)
RACER_OBJ = $(RACER_SRC:.cpp=.o)

# Executables
SERVER = server
CLIENT = client
RACER = thread_racer

all: $(SERVER) $(CLIENT) $(RACER)

$(SERVER): $(SERVER_OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(CLIENT): $(CLIENT_OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(RACER): $(RACER_OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SERVER_OBJ) $(CLIENT_OBJ) $(SERVER) $(CLIENT) $(RACER_OBJ) $(RACER)
