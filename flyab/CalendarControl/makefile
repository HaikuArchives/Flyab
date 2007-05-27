FLTK = fltk-1.1.7

# Haupttarget
MeinProgramm: CalendarControl.o TestApp.o
	g++ -O3 CalendarControl.o TestApp.o -o TestApp -L /usr/X11R6/lib -L../$(FLTK)/lib/ -lfltk -lXext -lX11 -lm

# TestApp.o target
TestApp.o: CalendarControl.o TestApp.cpp
	g++ -O3 -I. -I/usr/X11R6/include/X11 -I/usr/include -I../$(FLTK)/ -c TestApp.cpp -o TestApp.o

# CalendarControl.o target
#CalendarControl.o: CalendarControl.cpp
	#g++ -O3 -I. -I/usr/X11R6/include/X11 -I/usr/include -I../$(FLTK)/ -c CalendarControl.cpp -o CalendarControl.o

clean: 
	rm -f  *.o TestApp
