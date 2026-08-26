CXX ?= g++

CXXFLAGS := -g -O0 -fPIC \
	-fno-inline \
	-fno-omit-frame-pointer \
	-fno-optimize-sibling-calls \
	-pthread \
	-I$(JAVA_HOME)/include \
	-I$(JAVA_HOME)/include/linux

LDFLAGS := -shared -pthread

TARGET := libtrace_demo.so

all: java native

java:
	javac NativeBridge.java TraceDemo.java

native:
	$(CXX) $(CXXFLAGS) trace_demo.cpp $(LDFLAGS) -o $(TARGET)

clean:
	rm -f *.class
	rm -f *.so

.PHONY: all java native clean