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
	 gcc src/glibc-version.c -o bin/glibc-version.bin

testing.cpp: src/testing.cpp
	g++ src/testing.cpp -o bin/testing.bin

assert.cpp:  src/assert.cpp 
	 g++ src/assert.cpp -o bin/assert.bin

numberParse.cpp: src/numberParse.cpp 
	g++ src/numberParse.cpp -o numberParse.bin

#--------- Run test files --------- #

run-glibc-version:
	bin/glibc-version.bin

run-testing:
	bin/testing.bin

run-assert:
	bin/assert.bin

run-numberParse:
	bin/numberParse.bin

run:
	@echo "Running glibc-version.bin"
	bin/glibc-version.bin
	@echo
	@echo "-----------------------"

	#@echo "Running testing"
	#bin/testing.bin
	#@echo
	#@echo "-----------------------"

	@echo "Running assert"
	bin/assert.bin
	@echo
	@echo "-----------------------"

	@echo "Running numberParse"
	bin/numberParse.bin
	@echo
	@echo "-----------------------"



clean:
	rm -rf bin/*
