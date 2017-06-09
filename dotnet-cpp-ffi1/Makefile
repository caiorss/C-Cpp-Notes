lib: hpc.so

# Build shared library 
hpc.so: hpc.cpp
	g++ hpc.cpp -Wall -shared -fpic -o hpc.so
#	g++ hpc.cpp -o hpc.o -c -g -ggdb -fPIC -rdynamic -shared -lstatic 

# Build fsharp application
app.exe : wrapper.fsx
	fsharpc wrapper.fsx --out:app.exe --target:exe

run: hpc.so app.exe
	env LD_LIBRARY_PATH=. mono app.exe 

gsl: gsl.dll

libGslAdapter.so: gslAdapter.c
	gcc gslAdapter.c -o libGslAdapter.so -Wall -shared -fpic -lgsl -lgslcblas -lm 

gsl.dll: gsl.fsx libGslAdapter.so
	fsharpc gsl.fsx --out:gsl.dll --target:library

clean:
	rm -rf *.exe *.so *.bin *.dll
