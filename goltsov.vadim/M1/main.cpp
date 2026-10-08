#include <iostream>
#include <vector>
#include <tuple>

int main(int argc, char** argv) {

  if (argc != 3 && argc != 4) {
    std::cerr << "Invalid count command line arguments\n";
    return 1;
  }

  if (argv[1][0] == '-') {
    std::cerr << "Invalid threads argument. Threads must be a positive number\n";
    return 1;
  }
  size_t threads = std::stoull(argv[1]);

  if (argv[2][0] == '-') {
    std::cerr << "Invalid tries argument. Tries must be a positive number\n";
    return 1;
  }
  size_t tries = std::stoull(argv[2]);

  size_t seed = 0;
  if (argc == 4) {
    if (argv[3][0] == '-') {
      std::cerr << "Invalid seed argument. Seed must be a positive number\n";
      return 1;
    }
    seed = std::stoull(argv[2]);
  }

  std::vector< std::tuple< long long, long long, long long > > data;
  while (!std::cin.eof()) {
    long long r;
    if (!(std::cin >> r)) {
      if (std::cin.eof()) {
        break;
      }
      std::cerr << "Invalid data\n";
      return 1;
    }
    long long ignore;
    if (!(std::cin >> ignore)) {
      std::cerr << "Invalid data\n";
      return 1;
    }
    long long x;
    if (!(std::cin >> x)) {
      std::cerr << "Invalid data\n";
      return 1;
    }
    long long y;
    if (!(std::cin >> y)) {
      std::cerr << "Invalid data\n";
      return 1;
    }
    data.push_back(std::tuple< long, long, long >(r, x, y));
  }

  std::cout << threads << " " << tries << " " << seed << "\n";
  for (std::tuple< long long, long long, long long > i: data) {
    std::cout << std::get< 0 >(i) << " " << std::get< 1 >(i) << " " << std::get< 2 >(i) << "\n";
  }
}
