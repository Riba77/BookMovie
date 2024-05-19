
#include <iostream>
#include "cinemaplex.h"

using namespace cpx;

int main(int argc, char const *argv[]) {
  // Main function
  TheaterId theaterId("InitCommit");  // Call the constructor
  std::cout << "Theater ID: " << theaterId.id
            << std::endl;  // Print the theater ID
  return 0;
}
