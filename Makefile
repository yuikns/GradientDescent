.PHONY: all,run,clean
all: main.cc
	g++ -o GradientDescentDemo  main.cc --std=c++11

run: all
	./GradientDescentDemo

clean:
	rm -rf GradientDescentDemo *.o *.swp *~
	

