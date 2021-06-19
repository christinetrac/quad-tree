# List all the drivers
all: qtdriver
 
qtdriver: qttest.cpp
	g++ -std=c++11 -o  qtdriver qttest.cpp

# List all the executables under 'all:'
clean:
	rm qtdriver

