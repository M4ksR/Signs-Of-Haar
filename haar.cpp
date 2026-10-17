#include <fstream>
#include <iostream>

struct img {
  size_t w;
  size_t h;
};

int main(int argc, char* argv[]) {
  if (argc == 3) {
    std::ifstream infile(argv[1], std::ios_base::binary);
    std::ifstream input(argv[2]);
    if (!(infile && input)) {
      std::cout << "Can not open file" << std::endl;
      return 0;
    }

    uint16_t rast[2] = {};
    infile.read((char*)rast, 4);

    img i;
    i.w = rast[0];
    i.h = rast[1];

    uint8_t* list = new uint8_t[i.w * i.h];
    uint32_t* table = new uint32_t[(i.w + 1) * (i.h + 1)]();
    infile.read((char*)list, (std::streamsize)(i.w * i.h));

    for (size_t j = 0; j < i.w * i.h; ++j) {
      size_t index = (j / i.w + 1) * (i.w + 1) + (j % i.w + 1);
      table[index] = list[j] + table[index - (i.w + 1)] + table[index - 1] - table[index - (i.w + 2)];
    }
    infile.close();

    int count;
    input >> count;

    int x1;
    int y1;
    int x2;
    int y2;

    for (int j = 0; j < count; ++j) {
      input >> x1 >> y1 >> x2 >> y2;
      if (x1 < 1 || y1 < 1 || x2 < 1 || y2 < 1 || x1 > i.w || y1 > i.h || x2 > i.w || y2 > i.h || x1 > x2 || y1 > y2) {
        std::cout << "wrong box" << "\n";
        continue;
      }
      size_t leftu = (y1 - 1) * (i.w + 1) + x1 - 1;
      size_t rightu = (y1 - 1) * (i.w + 1) + x2;
      size_t leftd = y2 * (i.w + 1) + x1 - 1;
      size_t rightd = y2 * (i.w + 1) + x2;
      std::cout << table[rightd] - table[rightu] - table[leftd] + table[leftu] << "\n";
    }
    input.close();

    delete[] list;
    delete[] table;
  }
  return 0;
}