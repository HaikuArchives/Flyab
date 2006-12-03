##
## yab FLTK Makefile
##
## (c) Jan Bungeroth 2006 
## Artistic License. 
##
## Use 
##   make 
## to compile yab for FLTK.
##

##
## enable debug
##
## DBG = -g 
# 

##
## enable optimization
##
OPT = -O3

##
## Specify FLTK version and directory
##
FLTK = ../../fltk/

##
## GCC Options
##
GCC = gcc
GCC_OPT = $(DBG) $(OPT) -I. -I/usr/X11R6/include/X11 -I/usr/include -I$(FLTK) -DHAVE_CONFIG -DUNIX
GPP = g++
GPP_OPT = $(DBG) $(OPT) -I. -I/usr/X11R6/include/X11 -I/usr/include -I$(FLTK) -DHAVE_CONFIG -DUNIX 

##
## Libraries
##
# LIBPATH = -L/boot/home/config/lib  
#-L/boot/home/yab/CalendarControl.a
LIB = -lncurses -l pthread $(ZETALIB) -L /usr/X11R6/lib -L$(FLTK)lib/ -lfltk_images -lfltk_png -lfltk_jpeg -lfltk -lXext -lX11 -lXft -lm
#LIB = -lncurses -l pthread $(ZETALIB) -L /usr/X11R6/lib  -lXext -lX11 -lm

## flags for flex (-d for debugging)
FLEXFLAGS = -i -I -L -s

## flags for bison (-t -v for debugging)
BISONFLAGS = -d -l -t -v  

##
## Compile and link
##
yab: YabMain.o YabInterface.o YabView.o YabWindow.o YabColorControl.o YabAlert.o YabAlertWindow.o YabAlertText.o YabAlertIcon.o flex.o bison.o symbol.o function.o graphic.o io.o main.o
	$(GPP) $(GPP_OPT) -o yab YabMain.o YabInterface.o YabView.o YabWindow.o YabColorControl.o YabAlert.o YabAlertWindow.o YabAlertText.o YabAlertIcon.o main.o function.o io.o graphic.o symbol.o bison.o flex.o $(LIBPATH) $(LIB)

YabMain.o: YabMain.cpp 
	$(GPP) $(GPP_OPT) -c YabMain.cpp -o YabMain.o
YabInterface.o: YabInterface.cpp YabInterface.h BRect.h BPoint.h global.h YabMenuBar.h YabTabView.h YabStackView.h YabSplitView.h
	$(GPP) $(GPP_OPT) -c YabInterface.cpp -o YabInterface.o

YabView.o: YabView.cpp YabView.h
	$(GPP) $(GPP_OPT) -c YabView.cpp -o YabView.o
YabWindow.o: YabWindow.cpp YabWindow.h
	$(GPP) $(GPP_OPT) -c YabWindow.cpp -o YabWindow.o
#YabSplitView.o: YabSplitView.cpp YabSplitView.h
#	$(GPP) $(GPP_OPT) -c YabSplitView.cpp -o YabSplitView.o
YabColorControl.o: YabColorControl.cpp YabColorControl.h
	$(GPP) $(GPP_OPT) -c YabColorControl.cpp -o YabColorControl.o
#YabButton.o: YabButton.cpp YabButton.h
#	$(GPP) $(GPP_OPT) -c YabButton.cpp -o YabButton.o
#YabCheckButton.o: YabCheckButton.cpp YabCheckButton.h
#	$(GPP) $(GPP_OPT) -c YabCheckButton.cpp -o YabCheckButton.o
#YabRadioButton.o: YabRadioButton.cpp YabRadioButton.h
#	$(GPP) $(GPP_OPT) -c YabRadioButton.cpp -o YabRadioButton.o

YabAlert.o: YabAlert.cpp YabAlert.h
	$(GPP) $(GPP_OPT) -c YabAlert.cpp -o YabAlert.o
YabAlertWindow.o: YabAlertWindow.cpp YabAlertWindow.h
	$(GPP) $(GPP_OPT) -c YabAlertWindow.cpp -o YabAlertWindow.o
YabAlertText.o: YabAlertText.cpp YabAlertText.h
	$(GPP) $(GPP_OPT) -c YabAlertText.cpp -o YabAlertText.o
YabAlertIcon.o: YabAlertIcon.cpp YabAlertIcon.h imagedata.h global.h
	$(GPP) $(GPP_OPT) -c YabAlertIcon.cpp -o YabAlertIcon.o

#YabFilePanel.o: YabFilePanel.cpp YabFilePanel.h
#	$(GPP) $(GPP_OPT) -c YabFilePanel.cpp -o YabFilePanel.o

bison.o: bison.c yabasic.h config.h 
	$(GCC) $(GCC_OPT) -c bison.c -o bison.o
flex.o: flex.c bison.c yabasic.h config.h
	$(GCC) $(GCC_OPT) -c flex.c -o flex.o
function.o: function.c yabasic.h config.h
	$(GCC) $(GCC_OPT) -c function.c -o function.o
io.o: io.c yabasic.h config.h
	$(GCC) $(GCC_OPT) -c io.c -o io.o
graphic.o: graphic.c yabasic.h config.h
	$(GCC) $(GCC_OPT) -c graphic.c -o graphic.o
symbol.o: symbol.c yabasic.h config.h
	$(GCC) $(GCC_OPT) -c symbol.c -o symbol.o
main.o: main.c yabasic.h config.h 
	$(GCC) $(GCC_OPT) -c main.c -o main.o
flex.c: yabasic.flex 
	flex $(FLEXFLAGS) -t yabasic.flex >flex.c
	perl -i -n -e 'if (!/^\#include\s+<unistd.h>\s+$$/) {print if $$i;$$i++}' flex.c
	sed -e 's/int yylineno = 1;/ /' flex.c >flex2.c
	mv flex2.c flex.c
bison.c: yabasic.bison 
	bison $(BISONFLAGS) --output-file bison.c yabasic.bison  

clean:
	rm -f core *.o column/*.o flex.* bison.* yab
