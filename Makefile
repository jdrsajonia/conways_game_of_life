cxx=g++

cxxflags= -std=c++17 -pthread -static -static-libgcc -static-libstdc++

target=conway
src=conway.cpp

all:
	$(cxx) $(src) $(cxxflags) -o $(target) 


