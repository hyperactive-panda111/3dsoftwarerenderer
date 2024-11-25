# 3D Software Renderer

A 3D software renderer built from scratch in C, designed to explore the fundamentals of computer graphics. This project implements a complete rendering pipeline, including transformations, camera systems, perspective-correct texture mapping, lighting, and Z-buffering, without relying on a graphics API.

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

[Insert a GIF or link to a demo video]

## Table of Contents

- [Features](#features)
- [Demo](#demo)
- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [How It Works](#how-it-works)
- [Known Issues](#known-issues)
- [Future Enhancements](#future-enhancements)
- [License](#license)

## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/hyperactive-panda111/3d-software-renderer.git
   cd 3d-software-renderer
   ```

2. **Install dependencies**:
   - Ensure you have a C compiler (e.g., GCC)
   - Install libpng for handling PNG files:
     ```bash
     sudo apt-get install libpng-dev
     ```

3. **Build the project**:
   ```bash
   make
   ```

4. **Run the renderer**:
   ```bash
   ./renderer path/to/your/model.obj
   ```

## Usage

- Place your OBJ files and texture PNGs in the `models` directory
- Run the application with the path to the OBJ file
- Use keyboard inputs (if implemented) to control the camera or interact with the scene

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
├── assets/              # Screenshots or demo media
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

## Installation

### Install Dependencies

1. Ensure you have a C compiler (e.g., GCC)

### Build and Run

1. Build the project:
   ```bash
   make
   ```

2. Run the renderer:
   ```bash
   make run
   ```
## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Acknowledgments

- **Graphics Programming Inspiration**: Resources like Scratchapixel were instrumental in guiding this project
- Special thanks to the open-source community for providing tools and documentation
