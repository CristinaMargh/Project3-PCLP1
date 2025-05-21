# Image Processor in C

This is a command-line C application for loading, processing, and saving PGM/PPM images in ASCII or binary formats. It supports basic image operations such as selection, cropping, histogram analysis, equalization, rotation, and applying filters.

## Features

- Load and save `.pgm` / `.ppm` images (`P2`, `P3`, `P5`, `P6`)
- Select full image or custom rectangular regions
- Crop selected regions
- Generate grayscale histograms with configurable bin sizes
- Histogram equalization (grayscale only)
- Apply convolution filters:
  - EDGE
  - SHARPEN
  - BLUR
  - GAUSSIAN_BLUR
- Rotate full images or square selections by multiples of 90 degrees
- Save images in ASCII or binary format

## Supported File Types

- Grayscale (PGM):
  - `P2`: ASCII
  - `P5`: Binary
- Color (PPM):
  - `P3`: ASCII
  - `P6`: Binary

## Compilation

```bash
gcc -o image_editor image_editor.c
