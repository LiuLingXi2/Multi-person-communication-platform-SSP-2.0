.SSP: test

# protobuf
INC_PROTOBUF = -I/usr/local/include/google/protobuf
LIB_PROTOBUF = -L/usr/local/lib/libprotobuf.a -lprotobuf
OBJ_SSP = proto/*.o

# mysql
LIB_MYSQL = -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -ldl -lssl -lcrypto

test: client server

server: main.cc 
	g++ $^ -g -o ./bin/$@ -D _D $(INC_PROTOBUF) $(LIB_PROTOBUF) $(OBJ_SSP) $(LIB_MYSQL)

client: client.cc
	g++ $^ -g -o ./bin/$@ -D _D $(INC_PROTOBUF) $(LIB_PROTOBUF) $(OBJ_SSP)
