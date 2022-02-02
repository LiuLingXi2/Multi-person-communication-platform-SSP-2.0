.SSP: all

all: user rela mess photo db uuid

user: UserManager.cc
	g++ -c -o user.o $^

rela: RelationManager.cc
	g++ -c -o rela.o $^

mess: MessageManager.cc
	g++ -c -o mess.o $^

photo: PhotoManager.cc
	g++ -c -o photo.o $^

db: DbManager.cc
	g++ -c -o db.o $^

uuid: uuid.cc
	g++ -c -o uuid.o $^
	
.SSP: clean
clean: 
	rm -f *.o