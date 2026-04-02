# C Raytracer

A high-performance raytracer implemented in C that supports both Windows and Linux platforms. This raytracer features multithreading capabilities, BVH (Bounding Volume Hierarchy) acceleration, multiple material types, texturing, and various geometric primitives.

## Features

- Cross-platform support (Windows and Linux)
- Multithreading support for faster rendering
- Multiple primitive types (spheres, quads, triangles, circles)
- Various materials (Lambertian diffuse, metal, dielectric/glass)
- Texture support (checker patterns, image textures, Perlin noise)
- BVH (Bounding Volume Hierarchy) acceleration structure
- Output in PPM format with optional BMP conversion
- Flexible camera system with customizable parameters
- Support for different background types

## Prerequisites

### Windows
- Clang compiler
- Make build system
- Windows SDK (for Windows.h)

### Linux
- Clang compiler
- Make build system
- GCC build essentials

## Building the Project

### Windows Build Commands

```batch
# Generate object files and executable
./build.bat source [--mt] [--bmp]

# Clean all build artifacts
./build.bat clear

# Clean only executable files
./build.bat clear_bin

# Clean only object files
./build.bat clear_bin_int

# Show debug information
./build.bat info

# Run the raytracer
./build.bat render
```

### Linux Build Commands

```bash
# Generate object files and executable
./build.sh source [--mt] [--bmp]

# Clean all build artifacts
./build.sh clear

# Clean only executable files
./build.sh clear_bin

# Clean only object files
./build.sh clear_bin_int

# Show debug information
./build.sh info

# Run the raytracer
./build.sh render
```

### Build Options

- `--mt`: Enable multithreading support for faster rendering
- `--bmp`: Enable conversion of output PPM files to BMP format

## Project Structure

The raytracer is organized into several key components:

- `hittable.h`: Defines the interface for raytraced objects and implementations of various geometric primitives
- `material.h`: Contains material definitions including Lambertian diffuse, metal, and dielectric materials
- `texture.h`: Implements various texture types including checker patterns, image textures, and Perlin noise
- Other supporting files for mathematics, ray definitions, and utility functions

## Guidelines for Using the Raytracer

### Setting Up the Camera

The raytracer includes a customizable camera system that allows you to define:

- **Resolution**: Specify the width and height of the rendered image.
- **Field of View (FOV)**: Adjust the camera's perspective.
- **Position and Orientation**: Define the camera's location, the point it is looking at, and the "up" direction vector.

Example:
```c
camera* cam = camera_create(1920, 1080);
camera_render(cam, world, "output.ppm", 45, 
              (vec3){0, 0.5, 5},   // Camera position
              (vec3){0, 0.3, 0},   // Look-at point
              (vec3){0, 1, 0},     // Up vector
              256, 64,             // Samples per pixel, max ray depth
              background_default);
```

### Rendering Shapes

You can create and render a variety of shapes supported by the raytracer:

#### Supported Shape Properties

1. **Spheres**:
   - Center: `(x, y, z)` coordinates.
   - Radius: A positive float value.

   Example:
   ```c
   sphere my_sphere = sphere_create((point3){0.0, 0.0, 0.0}, 1.0, (material*)&my_material);
   hittable_list_add(world, (hittable*)&my_sphere);
   ```

2. **Quads**:
   - Origin: `(x, y, z)` coordinates.
   - Two vectors defining orientation and size.

   Example:
   ```c
   quad my_quad = quad_create((point3){-1.0, -1.0, 0.0}, 
                              (vec3){2.0, 0.0, 0.0}, 
                              (vec3){0.0, 2.0, 0.0}, 
                              (material*)&my_material);
   hittable_list_add(world, (hittable*)&my_quad);
   ```

3. **Boxes and Other Shapes**:
   Combine primitives with transformations like translation, rotation, and scaling.
   ```c
   box* my_box = box_create((point3){-1.0, -1.0, -1.0}, 
                            (point3){1.0, 1.0, 1.0}, 
                            (material*)&my_material);
   hittable_list_add(world, (hittable*)my_box);
   ```

### Using Transformations

Combine multiple transformations for advanced object positioning:
```c
translate translated_obj = translate_object((hittable*)&my_sphere, (vec3){1.0, 0.5, 0.0}, NULL);
rotate rotated_obj = rotate_object((hittable*)&translated_obj, (vec3){0.0, 1.0, 0.0}, DEG_TO_RAD(45), NULL);
hittable_list_add(world, (hittable*)&rotated_obj);
```

### Using Materials and Textures

Materials determine how objects interact with light. Supported types include:

- **Lambertian Diffuse**:
  ```c
  lambertian diffuse_material = lambertian_create((color){0.8, 0.3, 0.3}, NULL);
  ```

- **Metal**:
  Specify roughness for blurred reflections:
  ```c
  metal metallic_material = metal_create((color){0.8, 0.8, 0.8}, NULL, 0.1);
  ```

- **Dielectric (Glass)**:
  Set the refractive index for transparent objects:
  ```c
  dielectric glass_material = dielectric_create((color){1.0, 1.0, 1.0}, NULL, 1.5);
  ```

- **Textures**:
  Apply procedural or image-based textures to objects:
  ```c
  image_texture* img_tex = image_texture_create("path/to/image.jpg");
  lambertian textured_material = lambertian_create((color){0.0, 0.0, 0.0}, (texture*)img_tex);
  ```

### Lighting and Background

1. **Lighting**:
   Add light sources like diffuse lights:
   ```c
   diffuse_light my_light = diffuse_light_create((color){15, 15, 15}, NULL);
   quad light_quad = quad_create((point3){0, 10, 0}, (vec3){2, 0, 0}, (vec3){0, 2, 0}, (material*)&my_light);
   hittable_list_add(world, (hittable*)&light_quad);
   ```

2. **Background**:
   Customize the background color or gradient:
   ```c
   camera_render(cam, world, "output.ppm", 45, 
                 (vec3){0, 1, 5}, (vec3){0, 1, 0}, (vec3){0, 1, 0}, 
                 256, 64, background_default);
   ```

### Rendering the Scene

After defining the world, camera, and objects, render the scene:
```c
camera_render(cam, world, "output.ppm", 90, (vec3){0, 1, 5}, (vec3){0, 0, 0}, (vec3){0, 1, 0}, 128, 64, background_default);
```

### Cleanup

Destroy allocated objects and free memory after rendering:
```c
camera_destroy(cam);
hittable_list_destroy(world);
image_texture_destroy(img_tex);
```

### Error Handling Suggestions

1. Ensure all file paths for image textures are correct.
2. Verify camera parameters like field of view and position are valid.
3. Avoid overlapping objects unintentionally, as it might lead to visual artifacts.

### Optimization Tips for Large Scenes

1. Use the BVH acceleration structure for complex scenes with many objects.
2. Optimize memory usage by reusing textures and materials where possible.
3. Test rendering with a lower sample count to preview changes before increasing quality settings.

### Debugging Workflow

1. Enable debug logs in the build process to identify rendering errors.
2. Test individual components (like shapes, materials, or transformations) separately.
3. Validate the output scene step by step, ensuring each element renders as expected.

### Future Expansion Note

The raytracer can be extended in the following ways:

1. **Adding New Materials**: Implement custom shading models or specialized materials like subsurface scattering.
2. **Integrating New Shapes**: Expand the range of geometric primitives (e.g., toroids, cones).
3. **Enhancing Light Models**: Support advanced lighting techniques like global illumination or area lights.
4. **Optimizing Performance**: Leverage GPU-based rendering for faster computation.

## Output Formats

- Default output is in PPM (Portable Pixmap) format
- Optional conversion to BMP format when using the `--bmp` flag
- Output files are saved in the project directory

## Performance Tips

1. Use multithreading (`--mt` flag) for faster rendering on multi-core systems
2. The BVH acceleration structure significantly improves performance for scenes with many objects
3. Adjust sample count and maximum ray bounces based on desired quality vs. render time

## Troubleshooting

Common issues and solutions:

1. Build Errors:
   - Ensure all prerequisites are installed
   - Check compiler and Make installation
   - Verify Windows SDK installation (Windows only)

2. Runtime Errors:
   - Check file paths for image textures
   - Verify output directory permissions
   - Monitor system memory usage for complex scenes

3. Performance Issues:
   - Enable multithreading with `--mt` flag
   - Reduce sample count for faster preview renders
   - Use BVH for scenes with many objects

## Contributing

Feel free to contribute to this project by:

1. Reporting bugs
2. Suggesting new features
3. Submitting pull requests
4. Improving documentation

## License

This project is open source and available under the Apache License.

## Acknowledgments

This raytracer implementation is inspired by Peter Shirley's "Ray Tracing in One Weekend" series and other computer graphics resources.

