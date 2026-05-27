g++ -std=gnu++11 -IHeaders/ -c Sources/main.cpp -o lib/main.o -O9

g++ -std=gnu++11 -IHeaders/ -Wl,--whole-archive lib/Engine.a -Wl,--no-whole-archive lib/main.o -shared -o lib/Bots.dll -O9 -lPsapi -lgdi32 -luser32 -lmsvcrt

GameInjector.exe