# 3D Quadric Ray Tracer in C

A minimal ray tracer focused on rendering quadric surfaces (spheres, ellipsoids, hyperboloids, etc.) using the Phong illumination model.

## Project Objectives

This project implements a 3D ray tracer in C that demonstrates:

- Visualization of various quadric surfaces
- Ray-sphere intersection calculation
- Ray-general quadric intersection calculation
- Normal vector calculation on quadric surfaces
- Phong illumination model implementation
- Rendering different quadric types by only modifying coefficient matrices

## System Architecture

```
raytracer/
├── include/                # Header files
│   ├── utils.h             # Vector operations & utilities
│   └── quadrics.h          # Quadric surface handling
├── src/                    # Implementation files
│   ├── main.c              # Program entry & rendering logic
│   ├── utils.c             # Vector implementations & helpers
│   └── quadrics.c          # Quadric surface implementations
├── Makefile                # Build automation
└── README.md               # Project documentation
```

## Key Modules

- **Vector/Matrix Math**
  - 3D vectors, matrix operations (4x4 for quadrics)
  - Homogeneous coordinate transformations
- **Ray-Quadric Intersection**
  - Solves $[x,y,z,1]·Q·[x,y,z,1]^ᵀ = 0$ for ray $P = origin + t·direction$
  - Handles spheres, cones, hyperboloids via coefficient matrices
- **Phong Lighting**
  - Ambient/diffuse/specular components
  - Normal calculation: $\nabla f = 2·Q·[x,y,z,1]$ (first 3 components)
- **Rendering**
  - Pixel buffer for software rendering
  - PPM image output format

## Requirements

### Dependencies

- **Compilers:** GCC
- **Libraries:**
  - Standard C math library (libm)

## Building

```bash
# Build the ray tracer
make refactored

# Run the ray tracer (generates a PPM image)
./raytracer_refactored
```

## Example Quadric Matrices

| Surface               | 4x4 Matrix (Row-Major)                      |
|-----------------------|---------------------------------------------|
| Sphere                | diag(1, 1, 1, -r²)                          |
| Ellipsoid             | diag(1/a², 1/b², 1/c², -1)                  |
| One-sheet Hyperboloid | diag(1/a², 1/b², -1/c², -1)                 |
| Two-sheet Hyperboloid | diag(-1/a², -1/b², 1/c², -1)                |
| Elliptic Paraboloid   | diag(1/a², 1/b², 0, -1) with (0,0,1,-0) in row/col 3 |
| Hyperbolic Paraboloid | diag(1/a², -1/b², 0, 0) with (0,0,1,-0) in row/col 3 |
| Cylinder (z-axis)     | diag(1, 1, 0, -r²)                          |
| Cone (z-axis)         | diag(1/a², 1/b², -1/c², 0)                  |