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
## GCC Options
##
GCC = gcc
GCC_OPT = $(DBG) $(OPT) -I. -DHAVE_CONFIG -DUNIX
GPP = g++
GPP_OPT = $(DBG) $(OPT) -I. -DHAVE_CONFIG -DUNIX 

##
## Libraries
##
# LIBPATH = -L/boot/home/config/lib  
#-L/boot/home/yab/CalendarControl.a
LIB = -lncurses -lpthread -lfltk_images -lfltk -lm
#LIB = -lncurses -lpthread -lm

## flags for flex (-d for debugging)
FLEXFLAGS = -i -I -L -s

## flags for bison (-t -v for debugging)
BISONFLAGS = -d -l -t -v  

##
## Compile and link
##
yab: YabMain.o YabInterface.o YabView.o YabWindow.o YabSplitView.o YabColorControl.o YabButtonImage.o YabCheckboxImage.o YabAlert.o YabAlertWindow.o YabAlertText.o YabAlertIcon.o YabPopupMenu.o YabColumnBox.o flex.o bison.o symbol.o function.o graphic.o io.o main.o CalendarControl/CalendarControl.o
	$(GPP) $(GPP_OPT) -o yab YabMain.o YabInterface.o YabView.o YabWindow.o YabSplitView.o YabColorControl.o YabButtonImage.o YabCheckboxImage.o YabAlert.o YabAlertWindow.o YabAlertText.o YabAlertIcon.o YabPopupMenu.o YabColumnBox.o main.o function.o io.o graphic.o symbol.o bison.o flex.o CalendarControl/CalendarControl.o $(LIBPATH) $(LIB)

YabMain.o: YabMain.cpp 
	$(GPP) $(GPP_OPT) -c YabMain.cpp -o YabMain.o
YabInterface.o: YabInterface.cpp YabInterface.h BRect.h BPoint.h global.h YabMenuBar.h YabTabView.h YabStackView.h YabDropBox.h YabSlider.h
	$(GPP) $(GPP_OPT) -c YabInterface.cpp -o YabInterface.o

YabView.o: YabView.cpp YabView.h
	$(GPP) $(GPP_OPT) -c YabView.cpp -o YabView.o
YabWindow.o: YabWindow.cpp YabWindow.h
	$(GPP) $(GPP_OPT) -c YabWindow.cpp -o YabWindow.o
YabSplitView.o: YabSplitView.cpp YabSplitView.h
	$(GPP) $(GPP_OPT) -c YabSplitView.cpp -o YabSplitView.o
YabColorControl.o: YabColorControl.cpp YabColorControl.h
	$(GPP) $(GPP_OPT) -c YabColorControl.cpp -o YabColorControl.o
YabButtonImage.o: YabButtonImage.cpp YabButtonImage.h
	$(GPP) $(GPP_OPT) -c YabButtonImage.cpp -o YabButtonImage.o
YabCheckboxImage.o: YabCheckboxImage.cpp YabCheckboxImage.h
	$(GPP) $(GPP_OPT) -c YabCheckboxImage.cpp -o YabCheckboxImage.o
YabColumnBox.o: YabColumnBox.cpp YabColumnBox.h
	$(GPP) $(GPP_OPT) -c YabColumnBox.cpp -o YabColumnBox.o

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

CalendarControl/CalendarControl.o: CalendarControl/CalendarControl.cpp CalendarControl/CalendarControl.h
	$(GPP) $(GPP_OPT) -c CalendarControl/CalendarControl.cpp -o CalendarControl/CalendarControl.o

YabPopupMenu.o: YabPopupMenu.cpp YabPopupMenu.h
	$(GPP) $(GPP_OPT) -c YabPopupMenu.cpp -o YabPopupMenu.o

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
	rm -f core *.o CalendarControl/*.o flex.* bison.* yab
