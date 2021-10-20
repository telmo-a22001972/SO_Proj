OBJ_dir = obj
CC = gcc
OBJECTOS = main.o client.o memory-private.o memory.o process.o proxy.o server.o

out: $(OBJECTOS) 
	$(CC) $(addprefix $(OBJ_dir)/,$(OBJECTOS)) -o bin/out -lrt
%.o: src/%.c $($@) 
	$(CC) -I include -o $(OBJ_dir)/$@ -c $<