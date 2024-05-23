# BookMovie
The main objective of this project is to provide the implementation of a backend service in C++ which is used by some other services (CLI or other User Interface) for booking online movie tickets. Booking a ticket means just blocking the required number of particular seats.

# Prerequisites:
1. Install python 3.7 version or newer.
2. Install gcc compiler following: https://code.visualstudio.com/docs/cpp/config-mingw
2. Create virtual environment: ```py -m venv .venv```
3. Activate virtual enviorment: ```source .venv/Scripts/activate```
4. Install meson for building project: ```pip install meson```
5. Install ninja for building project: ```pip install ninja```

# Compile and run program:
1. Setup main program: ```meson setup Build```  
   Compile main program: ```meson compile -j8 -C Build cli -v``` 
   Compile tests: ```meson compile -j8 -C Build testprog -v``` 
2. Run program: ```Build/src/cli/cli.exe```
3. Run test program: ```Build/test/testprog.exe```

