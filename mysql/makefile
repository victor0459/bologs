SrcFiles=$(wildcard *.c)
TargetFiles=$(patsubst %.c,%,$(SrcFiles))

IncPath=/usr/include/mysql
LibPath=/usr/lib/x86_64-linux-gnu
PubLib=-lmysqlclient -ldl -lpthread -lrt

all:$(TargetFiles)

%:%.c
	gcc -o $@ $^ -I$(IncPath) -L$(LibPath) $(PubLib)

clean:
	-rm -f $(TargetFiles)
