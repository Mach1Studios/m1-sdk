First you need make libfdk-aac
Goto "fdk-aac" folder and execute:

./configure --prefix=/build/
make install

Copy libfdk-aac.1.dylib to "Lib" folder.
There are patched version with fix 263 line in  psy_main.cpp
