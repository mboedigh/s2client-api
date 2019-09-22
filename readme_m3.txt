Must have working git client

clone the git s2client-api repository 
	git clone --recursive https://github.com/Blizzard/s2client-api.git

Install cmake from the CMake webpage. Don't use the one with vs2019. Set the environment path to include the cmake binary

I edited CMakeLists.txt in the s2client-api to use C++17 
	set(CMAKE_CXX_STANDARD 17)

run these commands in a cmd window
	cmake -G "Visual Studio 16 2019" -B build 
	start build\s2client-api.sln

From visual studio 
I had so much trouble with this, that I build each project separately: contrib, sc2protocol, sc2api, sc2lib, sc2utils. 

I had to manually edit ip6-parser in my fork. (For reasons of my own, I have an updated v1.2 and it doesn't compile out of the box, the one that comes with s2client-api does not have a problem). 
Find and change the commented line to the uncommented version. 22 is the correct value, because it counts the null terminator after the string
	// const uint32_t IPV4_STRING_SIZE = sizeof "255.255.255.255:65535";
	#define IPV4_STRING_SIZE 22

sc2utils: compile with c++14 language standard
	right click on sc2utils project in the solution explorer (view...solution explorer) and select properties which opens property pages
	select c/c++...language.... Set c++language standard to c++14. 






