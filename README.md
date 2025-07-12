## Welcome to the match3 game. Made with Raylib and C.

Follow the intreuctions to compile the game for Linux and windows.

### CMake

- Clone the repo
- Type the follow commands:

```sh
cmake -S . -B build
```

> if you want with debug symbols put the flag `-DCMAKE_BUILD_TYPE=Debug`

- After CMake config your project build:

```sh
cmake --build build && ./build/match3/match3  
```

