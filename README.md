## Match3 Game

Match3 is a puzzle game built with Raylib and C.

### Build Instructions

#### Using CMake

1. Clone the repository:
    ```sh
    git clone https://github.com/nanda-gopal-sb/match3.git
    cd match3
    ```

2. Configure the project:
    ```sh
    cmake -S . -B build
    ```

    > For debug symbols, use the flag `-DCMAKE_BUILD_TYPE=Debug`.

3. Build and run:
    ```sh
    cmake --build build && ./build/match3/match3
    ```

### Install via DEB Package

1. Download the `.deb` file:
    ```sh
    wget https://github.com/nanda-gopal-sb/match3/releases/download/V1/match3-1.0.0-Linux.deb
    ```

2. Install the package:
    ```sh
    sudo dpkg -i match3-1.0.0-Linux.deb
    ```

3. Run the game:
    ```sh
    match3
    ```
