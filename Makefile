

build:
	scala -save build.scala -make all

# Copy src/ source code to dist directory.
code:
	cp -r -v src dist/

clean:
	scala -save build.scala -clean

