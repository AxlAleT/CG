#include <iostream>
#include <fstream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <width> <height>\n";
        return 1;
    }
    
    int width = std::atoi(argv[1]);
    int height = std::atoi(argv[2]);
    
    std::ofstream file("output.ppm");
    if (!file) {
        std::cerr << "Error opening file!\n";
        return 1;
    }
    
    file << "P3\n" << width << " " << height << "\n255\n";
    
    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            int r = static_cast<int>(255.0 * i / width);
            int g = static_cast<int>(255.0 * j / height);
            int b = static_cast<int>(255.0 * 0.2);
            file << r << " " << g << " " << b << " ";
        }
        file << "\n";
    }
    
    file.close();
    std::cout << "PPM image generated as output.ppm\n";
    return 0;
}
