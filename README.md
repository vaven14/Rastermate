# Rastermate

Rastermate is a tiny, dependency-free **software rasterizer** in C.
It provides a simple CPU framebuffer and drawing primitives for learning,
experiments, and headless rendering.

## Features
- Create/destroy buffer
- Clear buffer with color
- Set pixel (safe + fast)
- Blending pixel (safe + fast)

## Build
On Linux/macOS:

```bash
gcc examples/demo.c Rastermate.c -o demo
./demo
```
On Windows (MinGW):

```shell
gcc examples/demo.c Rastermate.c -o demo.exe
demo.exe
```