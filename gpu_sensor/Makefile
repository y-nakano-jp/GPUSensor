INC = -I/usr/include/NVCtrl/
CC = g++

program = test
$(program):$(program).o
	$(CC) ${INC} -lstdc++ -L/usr/lib  -lXext -lX11 -o  $(program) $(program).o /usr/lib/libXNVCtrl.a 

$(program).o:$(program).cpp
	$(CC) ${INC} -c -o $(program).o $(program).cpp

clean:
	rm -f *.o $(program)

