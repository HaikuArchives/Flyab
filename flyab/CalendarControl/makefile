FLTK = fltk-1.1.7

# Haupttarget
MeinProgramm: CalendarControl.o TestApp.o
	g++ -O3 CalendarControl.o TestApp.o -o TestApp -lfltk -lm

# TestApp.o target
TestApp.o: CalendarControl.o TestApp.cpp
	g++ -O3 -I. -c TestApp.cpp -o TestApp.o

clean: 
	rm -f  *.o TestApp
