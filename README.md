# Image Compression using Quadtree
## Project Overview
This project implements an image compression algorithm using a quadtree data structure. 
It features two different compression strategies (variance-based and count-based) to efficiently represent images while maintaining visual quality. 
The implementation demonstrates advanced C++ programming concepts including inheritance, recursive algorithms, and image processing techniques.

## Technical Implementation

### Core Components
1. **Quadtree Structure**
   - Recursive decomposition of images into square regions
   - Each node represents a region and stores:
     - Average color
     - Color variance
     - Dimensional information
     - Pointers to four child nodes (NW, NE, SW, SE)
       
2. **Compression Strategies**
   - **Count-based (qtcount)**
     - Merges regions based on pixel similarity count
     - Optimized for areas with similar colors
   
   - **Variance-based (qtvar)**
     - Merges regions based on color variance
     - Better preservation of detail in high-contrast areas

3. **Statistical Analysis**
   - Implements efficient algorithms for:
     - Color averaging
     - Variance calculation
     - Region similarity comparison
   - Uses cumulative sum tables for O(1) statistical queries
  
### Key Features

- **Adaptive Compression**: Automatically adjusts compression based on image content
- **Two-Phase Processing**: 
  1. Building phase: Creates full quadtree representation
  2. Pruning phase: Optimizes tree based on tolerance parameters
- **Efficient Memory Management**: Custom memory handling for large images
- **Modular Design**: Separate classes for tree structure, statistics, and compression logic

## Technical Skills Demonstrated

- Advanced Data Structures (Quadtrees)
- Object-Oriented Programming
- Memory Management
- Algorithm Design
- Image Processing
- C++ Programming
  - Inheritance
  - Operator Overloading
  - Template Programming
  - Smart Pointers


## Build and Usage


### Prerequisites
- C++ Compiler (C++14 or later)
- Make build system

### Building the Project
```bash
make
```

### Running the Program
```bash
./result
```

### Example Usage
```cpp
// Load image
PNG image;
image.readFromFile("input.png");

// Create and compress using count-based strategy
qtcount compressor(image);
compressor.prune(10000);  // Set compression tolerance

// Save compressed image
PNG result = compressor.render();
result.writeToFile("compressed.png");
```
## Algorithm Complexity

- **Build**: O(n) where n is the number of pixels
- **Compression**: O(log n) for each node evaluation
- **Render**: O(n) for final image generation
- **Space**: O(n) in worst case, O(k) after compression where k is number of remaining nodes

## Future Improvements

- Support for non-square images without cropping
- Parallel processing for large images
- Additional compression strategies
- Adaptive tolerance selection
- GUI interface for parameter tuning
