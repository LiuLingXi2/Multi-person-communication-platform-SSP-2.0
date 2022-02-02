.proto: all

all: proto user rela mess photo messt

proto:
	protoc --cpp_out=./ *.proto

user: 
	g++ -c -o user_pb.o UserInfo.pb.cc

rela:
	g++ -c -o rela_pb.o RelationInfo.pb.cc

mess:
	g++ -c -o mess_pb.o MessageInfo.pb.cc

photo:
	g++ -c -o photo_pb.o PhotoInfo.pb.cc

messt:
	g++ -c -o mess_pb.o MessType.pb.cc

clean:
	rm *.pb.h *.pb.cc *.o 