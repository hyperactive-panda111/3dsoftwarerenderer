# 3D Software Renderer

A 3D software renderer built from scratch in C, designed to explore the fundamentals of computer graphics. This project implements a complete rendering pipeline, including transformations, camera systems, perspective-correct texture mapping, lighting, and Z-buffering, without relying on a graphics API.

## Table of Contents

- [Features](#features)
- [Demo](#demo)
- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [How It Works](#how-it-works)
- [Known Issues](#known-issues)
- [Future Enhancements](#future-enhancements)
- [Further Reading](#further-reading)
- [License](#license)

## Features

- **3D Transformations**: Scale, rotate, and translate objects in 3D space
- **OBJ Model Import**: Load and render models in the Wavefront OBJ format
- **Texture Mapping**: Perspective-correct textures for realistic rendering
- **Camera Systems**: First-person and "look at" camera controls
- **Lighting**: Basic shading and lighting calculations for enhanced realism
- **Clipping**: Ensures objects outside the view frustum are not rendered
- **Z-Buffering**: Handles depth-based rendering for proper visibility
- **Multiple Mesh Rendering**: Support for scenes with multiple textured objects

## Demo
![Wireframe Mode](https://i.giphy.com/media/v1.Y2lkPTc5MGI3NjExNDlhMTRvOGYydWtiM3o4ZGVsbWJmb3U2NWlpdmljNzJydTB5YThtcyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/eDzhgYbI9QNBqdBWM0/giphy.gif)
![Wireframe Mode](https://i.giphy.com/media/v1.Y2lkPTc5MGI3NjExOTg5ZnFvbmc5bWl5Zm80eTB1aXM3ODIxMTBkanh6ajI5YXBiY2ZkMCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/nLr7jMBivNf5VcECJl/giphy.gif)
![Flat Shading Mode](https://i.giphy.com/media/v1.Y2lkPTc5MGI3NjExdW14b3V4OWRuYzJ5bHZ2bzBvdXU3bWhyeTB3ZzR0cjlzOGRmZWN1YiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/O2IgnbVemdm7nsvSGi/giphy.gif)
![Flat Shading Mode](https://i.giphy.com/media/v1.Y2lkPTc5MGI3NjExNjdqMWM2NGF2dmN0bTNzZmU5dWYwcWlsaG9qMzZzbmJiN3dpaDZpNyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/HhhSdDA9dUYijGsrrS/giphy.gif)
![Textured Mode](https://i.giphy.com/media/v1.Y2lkPTc5MGI3NjExOWJxa2Rtc3o4cWl3dG0zaXhwamJyZ2ViaXZkcnphdGttNG4xd3QyOSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/4cjPaXFR6GZCXMkJih/giphy.gif)
![Textured Mode](https://i.giphy.com/media/v1.Y2lkPTc5MGI3NjExcjBlZ2s4d3I5YjR0emhkNmk3bnU5d24xaTcya3E3Y2Nicjk3anc0ZiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/6YA3UVMR0PegQwpfEo/giphy.gif)
## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/hyperactive-panda111/3d-software-renderer.git
   cd 3d-software-renderer
   ```

2. **Install dependencies**:

- **C Compiler**: Ensure you have a C compiler like GCC installed.
- **Make**: Used to automate the build process. You can install it as follows:
  - On Ubuntu/Debian: `sudo apt install make`
  - On macOS: `xcode-select --install`
  - On Windows: Install [Make for Windows](http://gnuwin32.sourceforge.net/packages/make.htm) or use a Unix-like environment such as Cygwin or WSL.

3. **Build the project**:
   ```bash
   make
   ```

4. **Run the renderer**:
   ```bash
   ./renderer
   ```

## Camera Controls

The renderer allows interactive camera movement and adjustments through keyboard inputs. Below is a list of supported controls:

### Camera Rotation

- `W` - Rotate the camera upward (increase pitch)
- `S` - Rotate the camera downward (decrease pitch)
- `Right Arrow` - Rotate the camera clockwise (increase yaw)
- `Left Arrow` - Rotate the camera counterclockwise (decrease yaw)

### Camera Movement

- `Up Arrow` - Move the camera forward in the current direction
- `Down Arrow` - Move the camera backward in the current direction

### Rendering Modes

- `1` - Wireframe with vertex points
- `2` - Wireframe only
- `3` - Solid triangle fill
- `4` - Solid triangle fill with wireframe overlay
- `5` - Textured rendering
- `6` - Textured rendering with wireframe overlay

### Culling Methods

- `C` - Enable backface culling (default)
- `X` - Disable culling

### General

- `ESC` - Exit the program
## Project Structure

```
3d-software-renderer/
├── src/
│   ├── array.c           # Array utilities
│   ├── array.h
│   ├── camera.c          # Camera transformations
│   ├── camera.h
│   ├── clipping.c        # Clipping algorithms
│   ├── clipping.h
│   ├── display.c         # Display and frame buffer management
│   ├── display.h
│   ├── light.c           # Lighting calculations
│   ├── light.h
│   ├── main.c            # Entry point
│   ├── matrix.c          # Matrix operations
│   ├── matrix.h
│   ├── mesh.c            # Mesh handling and transformations
│   ├── mesh.h
│   ├── sort.c            # Sorting utilities (e.g., for Z-buffer)
│   ├── sort.h
│   ├── texture.c         # Texture loading and mapping
│   ├── texture.h
│   ├── triangle.c        # Triangle rasterization
│   ├── triangle.h
│   ├── upng.c            # PNG decoding
│   ├── upng.h
│   ├── vector.c          # Vector operations
│   ├── vector.h
├── assets/              # OBJ files
├── Makefile             # Build instructions
└── README.md            # Project documentation

```

## How It Works

### 1. Pipeline Overview

The renderer simulates a 3D graphics pipeline using only CPU-based calculations, mimicking how modern GPUs render 3D scenes. Key stages include:

- **Model Loading**: Parses Wavefront OBJ files to retrieve vertex data, texture coordinates, and face definitions
- **Transformations**: Applies matrix operations (translation, scaling, rotation) to position and orient models in 3D space
- **Projection**: Converts 3D coordinates into 2D screen space using perspective projection

### 2. Rendering Pipeline Stages

1. **Vertex Transformation**: Transforms model-space vertices into world-space and camera-space coordinates using the camera view matrix
2. **Clipping**: Ensures triangles outside the view frustum are excluded from further processing, improving performance and accuracy
3. **Rasterization**: Converts triangles into pixel fragments by mapping their positions to the frame buffer
4. **Depth Testing (Z-Buffer)**: Resolves overlapping objects by comparing and storing depth information for each pixel
5. **Texture Mapping**: Applies perspective-correct interpolation of texture coordinates, ensuring realistic rendering of surfaces

### 3. Lighting & Shading

- Basic directional lighting is implemented by calculating the dot product between surface normals and light directions
- Light intensity modifies vertex colors before rasterization for realistic shading

### 4. Camera System

- Includes both **First-Person** and **Look-At** modes, enabling dynamic scene interaction
- Uses a view matrix to transform world coordinates into camera-relative coordinates

### 5. File Handling

- **OBJ Loader**: Reads vertices, normals, texture coordinates, and face data to construct 3D models
- **PNG Decoder**: Loads texture images via a custom `upng` module

## Future Enhancements

- Add support for more advanced lighting techniques (e.g., Phong shading)
- Implement real-time rendering optimizations
- Introduce shader-like features for better material handling

## Further Reading

For a deep dive into the development process, pipeline stages, and technical challenges of this project, check out the accompanying blog post:

- [Building a 3D Software Renderer from Scratch](https://your-blog-link.com)

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Acknowledgments

- **Graphics Programming Inspiration**: Resources like Scratchapixel were instrumental in guiding this project
- Special thanks to the open-source community for providing tools and documentation
