# Fl_Table Makefile
FLTKDIR=/usr/local/src/fltk-1.1.7
#FLTKDIR=/usr/local/src/fltk-utf8-1.1.4
SHELL=/bin/csh -f

# HANDLE INCLUDING OS SPECIFIC MAKEFILE
OS=$(shell uname | tr a-z A-Z | sed 's/_.*//')
include Makefile.$(OS)

BINS=testtablerow testtablerow-utf8 testkeyboardnav \
     exercisetablerow widgettable singleinput sortapp 

all: $(BINS)

clean:
	-rm -f *.o ._* *.obj *.exe core core.* foo singleinput-tabs $(BINS)

OBJS=Fl_Table.o Fl_Table_Row.o

Fl_Table.o: Fl_Table.cxx
	$(CC) $(CFLAGS) -I$(FLTKDIR) -c Fl_Table.cxx

Fl_Table_Row.o: Fl_Table.o Fl_Table_Row.cxx
	$(CC) $(CFLAGS) -I$(FLTKDIR) -c Fl_Table_Row.cxx

table: table.cxx $(OBJS)
	$(CC) $(CFLAGS) -I$(FLTKDIR) table.cxx -o table $(OBJS) $(FLTKLIBS)
	$(REZCOMMAND) table

testkeyboardnav: testkeyboardnav.cxx $(OBJS)
	$(CC) $(CFLAGS) -I$(FLTKDIR) testkeyboardnav.cxx -o testkeyboardnav \
		$(OBJS) $(FLTKLIBS)
	$(REZCOMMAND) testkeyboardnav

testtablerow: testtablerow.cxx $(OBJS)
	$(CC) $(CFLAGS) -I$(FLTKDIR) testtablerow.cxx -o testtablerow $(OBJS) $(FLTKLIBS)
	$(REZCOMMAND) testtablerow

testtablerow-utf8: testtablerow-utf8.cxx $(OBJS)
	$(CC) $(CFLAGS) -I$(FLTKDIR) testtablerow-utf8.cxx -o testtablerow-utf8 $(OBJS) $(FLTKLIBS)
	$(REZCOMMAND) testtablerow-utf8

sortapp: sortapp.cxx $(OBJS)
	$(CC) $(CFLAGS) -I$(FLTKDIR) sortapp.cxx -o sortapp $(OBJS) $(FLTKLIBS)
	$(REZCOMMAND) sortapp

exercisetablerow: exercisetablerow.cxx $(OBJS)
	$(CC) $(CFLAGS) -I$(FLTKDIR) exercisetablerow.cxx -o exercisetablerow $(OBJS) $(FLTKLIBS)
	$(REZCOMMAND) exercisetablerow

widgettable: widgettable.cxx $(OBJS)
	$(CC) $(CFLAGS) -I$(FLTKDIR) widgettable.cxx -o widgettable $(OBJS) $(FLTKLIBS)
	$(REZCOMMAND) widgettable

singleinput: singleinput.cxx $(OBJS)
	$(CC) $(CFLAGS) -I$(FLTKDIR) singleinput.cxx -o singleinput $(OBJS) $(FLTKLIBS)
	$(REZCOMMAND) singleinput

singleinput-tabs: singleinput-tabs.cxx $(OBJS)
	$(CC) $(CFLAGS) -I$(FLTKDIR) singleinput-tabs.cxx -o singleinput-tabs $(OBJS) $(FLTKLIBS)
	$(REZCOMMAND) singleinput-tabs

# For quickie tests
foo: foo.cxx $(OBJS)
	$(CC) $(CFLAGS) -I$(FLTKDIR) foo.cxx -o foo $(OBJS) $(FLTKLIBS)
	$(REZCOMMAND) foo

tar: FORCE
	make clean
#	( cd .. ; tar cvfz Fl_Table.tar.gz Fl_Table/* )
	( set VERSION=`awk '/^[0-9]*\.[^ \t]/ {print $$1; exit(0);}'<CHANGES`; \
	  set TARFILE=/var/tmp/Fl_Table-$$VERSION.tar.gz; \
	  set RELDIR=.release/Fl_Table-$$VERSION; \
	  rm -rf documentation/images/.xvpics; \
	  rm -rf .release; mkdir -p $$RELDIR; \
	  cp *.cxx *.H *.h Make* make.bat CHANGES COPYING CREDITS TODO README* $$RELDIR; \
	  cp -rp documentation FL $$RELDIR; \
	  find .release -type d -exec chmod 755 {} \; ; \
	  find .release -type f -exec chmod 644 {} \; ; \
	  ( cd .release && tar cvfz $$TARFILE --numeric-owner --owner=0 --group=0 Fl_Table-$$VERSION ); \
	  echo "*** Created: $$TARFILE"; \
	)

# DON'T REMOVE
FORCE:
	
