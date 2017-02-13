#
# To compile all c-files enter:
#   $ make c-files 
#
# To compile all cpp/c++ files enter:
#   $ make cpp-files
#
all:       cpp-files c-files 
cpp-files: testing.cpp assert.cpp numberParse.cpp
c-files:   glic-version.c


glic-version.c:
	 mkdir -p bin
	 gcc src/glibc-version.c -o bin/glibc-version

testing.cpp: src/testing.cpp
	g++ src/testing.cpp -o bin/testing.bin

assert.cpp:  src/assert.cpp 
	 g++ src/assert.cpp -o bin/assert.bin

numberParse.cpp: src/numberParse.cpp 
	g++ src/numberParse.cpp -o numberParse.bin 

clean:
	rm -rf bin/*
