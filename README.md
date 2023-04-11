# Tetris-openGL
Tetris written in C++ using OpenGL

# How to build 
## Linux
1. Install GLFW and GLM
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

3. Clone repo and two other dependencies
```sh
git clone https://github.com/somso2e/Tetris-openGL.git
cd Tetris-openGL

mkdir -p dependencies/stb/
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h -P dependencies/stb

mkdir -p dependencies/nlohmann
wget https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp -P dependencies/nlohmann

```

4. Build using CMake

```sh

```

## Windows

soon ™️

## MacOS

lol good one