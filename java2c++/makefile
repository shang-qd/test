.PHONY : all clean run 

inc := -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux
TARGET := libDangerLogic.so
  
SRCS := $(wildcard *.cpp)  
OBJS := $(patsubst %cpp,%o,$(SRCS))

Q ?= @

all : DangerLogic.class $(TARGET)

DangerLogic.class : DangerLogic.java
	javac $<
	javah -jni DangerLogic
  
%.o : %.cpp
	@ echo -o $@
	$(Q) g++ -std=c++11 -fPIC -c $(inc) $^  

$(TARGET) : $(OBJS) 
	@ echo "\033[32m"CXX/LD -o $@ "\033[0m"
	$(Q) g++ -std=c++11 -shared $(inc) $^ -o $@
	sudo cp $@ /usr/lib/

clean : 
	rm -rf $(TARGET) *.o
	rm -rf DangerLogic.h
	rm -rf DangerLogic.class
	sudo rm -rf /usr/lib/$(TARGET)

run : all
	@ echo "java begin call c++"
	@ java DangerLogic
