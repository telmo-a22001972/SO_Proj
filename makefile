OBJ_dir = obj
CC = gcc
OBJECTOS = main.o client.o memory-private.o memory.o process.o proxy.o server.o synchronization.o log.o configuration.o sotime.o

out: $(OBJECTOS) 
	$(CC) $(addprefix $(OBJ_dir)/,$(OBJECTOS)) -o bin/sovac -lrt -lpthread
%.o: src/%.c $($@) 
	$(CC) -g -I include  -o $(OBJ_dir)/$@ -c $<
