#include <iostream>
#include <fstream>
#include <cstdlib>
#include <array>

#define CAFE {41, 0, 3}
#define ROJO {255, 0, 0}
#define BLANCO {255, 255, 255}
#define PIEL {255, 219, 172}

int main(int argc, char* argv[]) {
    int width = 16;
    int height = 16;

    std::ofstream file("mario.ppm");
    if (!file) {
        std::cerr << "Error opening output file!\n";
        return 1;
    }

    std::ifstream input_file("input.txt");
    if (!input_file) {
        std::cerr << "Error opening input file!\n";
        return 1;
    }

    file << "P3\n" << width << " " << height << "\n255\n";

    char pixel;
    std::array<int, 3> color;
    
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            input_file >> pixel;
            switch (pixel) {
                case 'W':
                    color = BLANCO;
                    break;
                case 'R':
                    color = ROJO;
                    break;
                case 'C':
                    color = CAFE;
                    break;
                case 'P':
                    color = PIEL;
                    break;
                default:
                    color = {0, 0, 0};
                    break;
            }
            file << color[0] << " " << color[1] << " " << color[2] << " ";
        }
        file << "\n";
    }

    file.close();
    input_file.close();
    std::cout << "PPM image generated as mario.ppm\n";
    return 0;
}
