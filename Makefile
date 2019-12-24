
make:
	g++ main.cpp visuals.h visuals.cpp -lGL -lglut -lGLU -o exe
clean:
	rm -rf *.o
	rm -rf exe
run:
	./exe

