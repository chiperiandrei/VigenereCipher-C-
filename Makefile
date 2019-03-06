#fisier folosit pentru compilarea serverului&clientului TCP iterativ

all:
	gcc main.c -o decriptare
	g++ encrypt.cpp -o criptare
clean:
	rm -f *~ decriptare criptare
run:
	./criptare QWERTY
	./decriptare
