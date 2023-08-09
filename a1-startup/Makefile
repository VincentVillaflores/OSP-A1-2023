
##
 # startup code provided by Paul Miller for COSC1114 - Operating Systems
 # Principles
 ##

# provide make targets here to build the two programs 
all: copier mtcopier

copier: copier_files/reader.cpp copier_files/writer.cpp copier_files/main.cpp
	g++ -o copier copier_files/reader.cpp copier_files/writer.cpp copier_files/main.cpp -Wall -Werror -std=c++20 -g

mtcopier: mtcopier_files/reader.cpp mtcopier_files/writer.cpp mtcopier_files/main.cpp
	g++ -o mtcopier mtcopier_files/reader.cpp mtcopier_files/writer.cpp mtcopier_files/main.cpp -Wall -Werror -std=c++20 -g -lpthread
