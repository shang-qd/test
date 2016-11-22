.PHONY : all run clean

inc := -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux

all : HelloWorld.class libHelloWorld.so
	

HelloWorld.class : HelloWorld.java
	javac $<
	javah -jni HelloWorld

libHelloWorld.so : HelloWorld.c
	g++ -fPIC -shared -std=c++11 $(inc) $< -o $@
	sudo cp $@ /usr/lib/

run :
	java HelloWorld

clean :
	rm -f HelloWorld.class
	rm -f HelloWorld.h
	rm -f libHelloWorld.so
	sudo rm -f /usr/lib/libHelloWorld.so

