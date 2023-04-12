# Tetris-openGL
Tetris written in C++ using OpenGL

# How to build 
## Linux
1. Install GLFW and GLM (Debian-based distros)
```sh
sudo apt install libglfw3-dev
sudo apt install libglm-dev 
```

2.  Build Freetype from source and install if it's not installed already
```sh
git clone https://git.savannah.nongnu.org/git/freetype/freetype2.git
cd freetype2
mkdir build && cd build
cmake ..     
make         
make install 
```

3. Build using CMake

```sh
git clone --recurse-submodules https://github.com/somso2e/Tetris-openGL.git
mkdir build && cd build
cmake ..     
make
```

## Windows

soon ™️

## MacOS

lol good one