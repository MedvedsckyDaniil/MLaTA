all: MLaTA clean

MLaTA: main.o conclude.o converter.o to_str.o
	g++ main.o conclude.o converter.o to_str.o -o MLaTA.exe

main.o: main.cpp conclude.h
	g++ -c main.cpp

conclude.o: conclude.cpp converter.h to_str.h
	g++ -c conclude.cpp

converter.o: converter.cpp converter.h
	g++ -c converter.cpp

to_str.o: to_str.cpp to_str.h
	g++ -c to_str.cpp

clean: MLaTA
	rm -f *.o
