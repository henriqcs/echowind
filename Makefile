inc_dir = include
obj_dir = obj
src_dir = src

CC = gcc
CFLAGS = -I$(inc_dir)

_deps = unix_sockets.h error_functions.h ename.c.inc
deps = $(patsubst %,$(inc_dir)/%,$(_deps))

_client_obj = error_functions.o unix_client_socket.o
client_obj = $(patsubst %,$(obj_dir)/%,$(_client_obj))

_server_obj = error_functions.o unix_server_socket.o
server_obj = $(patsubst %,$(obj_dir)/%,$(_server_obj))

$(obj_dir)/%.o: $(src_dir)/%.c $(deps)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(server_obj)
	$(CC) -o $@ $^

client: $(client_obj)
	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm -f $(obj_dir)/*.o $(inc_dir)/*~ *~ core
