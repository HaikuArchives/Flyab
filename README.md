flyab is still highly experimental.
Edit the Makefile to put in the correct path for the fltk directory. You need a 1.1.x version of fltk.
You have to configure fltk with the following options:
```
./configure --enable-threads --enable-xft --enable-localjpeg --enable-localpng
```
(xft can be omitted)

Originally from: http://sourceforge.net/projects/yab-interpreter/
License: Artistic or GPL