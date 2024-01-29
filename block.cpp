#include "block.h"
#include <cmath>
#include <iostream>

int Block::height() const {
    return data.at(0).size();
  }

int Block::width() const {
  return data.size();
}

void Block::render(PNG &im, int x) const {
  for (int i = x; i < width() + x; i++) {
    for (int j = 0; j < height(); j++) {
      vector<HSLAPixel> vec = data[i-x];
      *(im.getPixel(i,j)) = vec[j];
    }
  }
}

void Block::build(PNG &im, int x, int width) {
  data.clear();
  for (int i = x; i < width + x; i++) {
    vector<HSLAPixel> vertical_vec;
    for (unsigned int j = 0; j < im.height(); j++) {
      HSLAPixel * pixel = im.getPixel(i,j);
      vertical_vec.push_back(* pixel);
    }
    data.push_back(vertical_vec);
  }
}
