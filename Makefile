


glic-version:
	 mkdir -p bin
	 gcc src/glibc-version.c -o bin/glibc-version
	 ./bin/glibc-version

clean:
	rm -rf bin/*
