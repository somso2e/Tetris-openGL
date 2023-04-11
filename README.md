# Tetris-openGL
Tetris written in C++ using OpenGL

# How to build 
## Linux
1. Install GLFW and GLM (Debian-based distros)
```sh
sudo apt install libglfw3-dev
sudo apt install libglm-dev 
```

2.  Build Freetype
```sh
git clone https://git.savannah.nongnu.org/git/freetype/freetype2.git
cd freetype2
mkdir build && cd build
cmake ..     
make         
make install 
```
3. Download glad

    1. Go to https://glad.dav1d.de/
    2. Only change gl version to version 4.0 (Dont change anything else) and hit Generate
    3. Download the `glad.zip` file
    4. Extract the files in to /dependencies/glad


4. Clone this repo and two other dependencies
```sh
git clone https://github.com/somso2e/Tetris-openGL.git
cd Tetris-openGL

mkdir -p dependencies/stb/
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h -P dependencies/stb

mkdir -p dependencies/nlohmann
wget https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp -P dependencies/nlohmann

```



5. Build using CMake

```sh
mkdir build && cd build
cmake ..     
make
```

## Windows

soon ™️

## MacOS

lol good one