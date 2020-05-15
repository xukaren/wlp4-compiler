wlp4gen: wlp4gen.o 
	g++ wlp4gen.o -o wlp4gen

wlp4gen.o: wlp4gen.cc  wlp4gen.h 
	g++ -std=c++14 -Wall -g -c wlp4gen.cc

.PHONY: clean

clean:
	rm *.o wlp4gen
	
