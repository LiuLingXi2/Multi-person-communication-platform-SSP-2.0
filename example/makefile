.SSP: test

# protobuf
INC_PROTOBUF = -I/usr/local/include/google/protobuf
LIB_PROTOBUF = -L/usr/local/lib/libprotobuf.a -lprotobuf
OBJ_SSP = ../proto/*.o

# mysql
LIB_MYSQL = -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -ldl -lssl -lcrypto

test: server_test client_test

server_test: server.cc ../src/*.cc ../network/*.cc
	g++ $^ -g -o bin/$@ -D _D $(INC_PROTOBUF) $(LIB_PROTOBUF) $(OBJ_SSP) $(LIB_MYSQL)

client_test: client.cc ../src/*.cc ../network/*.cc
	g++ $^ -g -o bin/$@ -D _D $(INC_PROTOBUF) $(LIB_PROTOBUF) $(OBJ_SSP) $(LIB_MYSQL)

clean:
	rm -f ./bin/client_test ./bin/server_test
