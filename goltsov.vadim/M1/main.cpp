#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <thread>
#include <future>
#include <numeric>
#include <utility>

namespace goltsov {
  bool isInside(const std::tuple< double, double, double >& s, const double& x, const double& y);
  std::pair< size_t, size_t > calc(const std::vector< std::tuple< double, double, double > >& data,
    const size_t& tests, const size_t& seed, const double& minX, const double& minY, const double& maxX,
    const double& maxY);
  std::pair< double, double > areas(const std::vector< std::tuple< double, double, double > >& data,
    const size_t& threads, const size_t& tries, const size_t& start_seed, const double& minX,
    const double& minY, const double& maxX, const double& maxY);
}

int main(int argc, char** argv) {

  if (argc != 3 && argc != 4) {
    std::cerr << "Invalid count command line arguments\n";
    return 1;
  }

  if (argv[1][0] == '-') {
    std::cerr << "Invalid threads argument. Threads must be a not negative number\n";
    return 1;
  }
  size_t threads = std::stoull(argv[1]);

  if (argv[2][0] == '-') {
    std::cerr << "Invalid tries argument. Tries must be a positive number\n";
    return 1;
  }
  size_t tries = std::stoull(argv[2]);
  if (tries == 0) {
    std::cerr << "Invalid tries argument. Tries must be a positive number\n";
    return 1;
  }

  size_t seed = 0;
  if (argc == 4) {
    if (argv[3][0] == '-') {
      std::cerr << "Invalid seed argument. Seed must be a positive number\n";
      return 1;
    }
    seed = std::stoull(argv[2]);
  }

  double minX = std::numeric_limits< double >::max();
  double minY = std::numeric_limits< double >::max();
  double maxX = std::numeric_limits< double >::min();
  double maxY = std::numeric_limits< double >::min();

  std::vector< std::tuple< double, double, double > > data;
  while (!std::cin.eof()) {
    double r;
    if (!(std::cin >> r)) {
      if (std::cin.eof()) {
        break;
      }
      std::cerr << "Invalid data\n";
      return 1;
    }
    double ignore;
    if (!(std::cin >> ignore)) {
      std::cerr << "Invalid data\n";
      return 1;
    }
    double x;
    if (!(std::cin >> x)) {
      std::cerr << "Invalid data\n";
      return 1;
    }
    double y;
    if (!(std::cin >> y)) {
      std::cerr << "Invalid data\n";
      return 1;
    }
    minX = std::min(minX, x - r);
    minY = std::min(minY, y - r);
    maxX = std::max(maxX, x + r);
    maxY = std::max(maxY, y + r);
    data.push_back(std::tuple< double, double, double >(r, x, y));
  }

  std::pair< double, double > areas = goltsov::areas(data, threads, tries, seed, minX, minY, maxX, maxY);
  std::cout << areas.first << " " << areas.second << "\n";
}

bool goltsov::isInside(const std::tuple< double, double, double >& s, const double& x, const double& y) {
  double r = std::get< 0 >(s);
  double sx = std::get< 1 >(s);
  double sy = std::get< 2 >(s);
  return r * r >= (x - sx) * (x - sx) + (y - sy) * (y - sy);
}


std::pair< size_t, size_t > goltsov::calc(const std::vector< std::tuple< double, double, double > >& data,
  const size_t& tests, const size_t& seed, const double& minX, const double& minY, const double& maxX,
  const double& maxY) {
  std::default_random_engine engine = std::default_random_engine(seed);
  std::uniform_real_distribution< double > distX(minX, maxX);
  std::uniform_real_distribution< double > distY(minY, maxY);
  size_t count_inside = 0;
  size_t count_inside_in_all = 0;
  for (int i = 0; i < tests; ++i) {
    double x = distX(engine);
    double y = distY(engine);
    bool is_inside = false;
    bool is_inside_in_all = true;
    for (int j = 0; j < data.size(); ++j) {
      if (isInside(data[j], x, y)) {
        is_inside = true;
      } else {
        is_inside_in_all = false;
      }
    }
    if (is_inside) {
      count_inside++;
    }
    if (is_inside_in_all) {
      count_inside_in_all++;
    }
  }
  return std::pair< size_t, size_t >(count_inside, count_inside_in_all);
}

std::pair< double, double > goltsov::areas(const std::vector< std::tuple< double, double, double > >& data,
  const size_t& threads, const size_t& tries, const size_t& start_seed, const double& minX, const double& minY,
  const double& maxX, const double& maxY) {
  std::vector< std::future< std::pair< size_t, size_t > > > results_in_threads =
    std::vector< std::future< std::pair< size_t, size_t > > >(threads);
  for (int i = 0; i < tries % threads; ++i) {
    results_in_threads[i] = std::async(std::launch::async, calc, data, tries / threads + 1, start_seed + i, minX,
      minY, maxX, maxY);
  }
  for (int i = tries % threads; i < threads; ++i) {
    results_in_threads[i] = std::async(std::launch::async, calc, data, tries / threads, start_seed + i, minX, minY,
      maxX, maxY);
  }
  size_t count_inside = 0;
  size_t count_inside_in_all = 0;
  for (int i = 0; i < threads; ++i) {
    std::pair< size_t, size_t > result_i = results_in_threads[i].get();
    count_inside += result_i.first;
    count_inside_in_all += result_i.second;
  }
  return std::pair< double, double >(
    (maxX - minX) * (maxY - minY) * ((static_cast< double >(count_inside)) / static_cast< double >(tries)),
    (maxX - minX) * (maxY - minY) * ((static_cast< double >(count_inside_in_all)) / static_cast< double >(tries))
  );
}
