.SSP: all

# protobuf
INC_PROTOBUF = -I/usr/local/include/google/protobuf
LIB_PROTOBUF = -L/usr/local/lib/libprotobuf.a -lprotobuf
OBJ_SSP = proto/*.o

# mysql
LIB_MYSQL = -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -ldl -lssl -lcrypto

all: server client

server: main.cc ./src/*.cc
	g++ $^ -g -o ./bin/$@ -D _D $(INC_PROTOBUF) $(LIB_PROTOBUF) $(OBJ_SSP) $(LIB_MYSQL)
