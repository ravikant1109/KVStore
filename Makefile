
all: gendir client server kvstore xmlParser kvcache

CC = gcc

OBJDIR = obj
BUILD_DIR = build
SERVER_DIR = ServerData
OBJ_SERVER = $(OBJDIR)/server.o 
OBJ_CLIENT = $(OBJDIR)/client.o
OBJ_KVSTORE = $(OBJDIR)/kvstore.o
OBJ_KVCACHE = $(OBJDIR)/kvcache.o
OBJ_XML = $(OBJDIR)/xmlParserRunner.o
OBJ_THREADPOOL = $(OBJDIR)/threadpool.o 

HEADERDIR = include
HEADER = $(HEADERDIR)/*.h 
CFLAGS = -c -Iinclude/

gendir:
	mkdir -p $(OBJDIR)
	mkdir -p $(SERVER_DIR)
	mkdir -p $(BUILD_DIR)

# .PHONY: client
client: $(BUILD_DIR)/client

$(OBJ_CLIENT): src/client.c $(HEADER)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/client: $(OBJ_CLIENT)
	$(CC) $(OBJ_CLIENT) -o $@

xmlParser: $(BUILD_DIR)/xmlParser

$(OBJ_XML): src/xmlParserRunner.c $(HEADER)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/xmlParser: $(OBJ_XML)
	$(CC) $(OBJ_XML) -o $@


# threadpool: $(BUILD_DIR)/threadpool

# $(OBJ_THREADPOOL): src/threadpoolRunner.c $(HEADER)
# 	$(CC) $(CFLAGS) $< -o $@

# $(BUILD_DIR)/threadpool: $(OBJ_THREADPOOL)
# 	$(CC) $(OBJ_THREADPOOL) -o $@ -lpthread


server: $(BUILD_DIR)/server

$(OBJ_SERVER): src/server.c $(HEADER)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/server: $(OBJ_SERVER)
	$(CC) $(OBJ_SERVER) -o $@ -lpthread

kvstore: $(BUILD_DIR)/kvstore

$(OBJ_KVSTORE): src/kvstoreRunner.c $(HEADER)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/kvstore: $(OBJ_KVSTORE)
	$(CC) $(OBJ_KVSTORE) -o $@


kvcache: $(BUILD_DIR)/kvcache

$(OBJ_KVCACHE): src/kvcacheRunner.c $(HEADER)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/kvcache: $(OBJ_KVCACHE)
	$(CC) $(OBJ_KVCACHE) -o $@

clean: 
	@rm -rf *.out obj/* build/*
	@rmdir obj build
