# Voxel Renderer
A Minecraft-esque voxel renderer written in C++ and OpenGL.

![screenshot](images/screenshot.png)

## Dependencies
### Linux (Ubuntu)
```bash
sudo apt install libwayland-dev libxkbcommon-dev xorg-dev
```

## Building
```bash
git clone https://github.com/justint9696/voxel-renderer.git --recurse-submodules
cd voxel-renderer
cmake -B build -S .
cmake --build build
```

After a successful build has been made, the application can be ran from the **root directory** with `build/game`.
