#include <iostream>

int main(int argc, char** argv) {

  if (argc != 3 || argc != 4) {
    std::cerr << "Invalid count command line arguments";
    return 1;
  }

  if (argv[1][0] == '-') {
    std::cerr << "Invalid threads argument. Threads must be a positive number";
    return 1;
  }
  size_t threads = std::stoull(argv[1]);

  if (argv[2][0] == '-') {
    std::cerr << "Invalid tries argument. Tries must be a positive number";
    return 1;
  }
  size_t tries = std::stoull(argv[2]);

  size_t seed = 0;
  if (argc == 4) {
    if (argv[3][0] == '-') {
      std::cerr << "Invalid seed argument. Seed must be a positive number";
      return 1;
    }
    seed = std::stoull(argv[2]);
  }
}
