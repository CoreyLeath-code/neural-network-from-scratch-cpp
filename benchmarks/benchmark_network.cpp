#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

#include "core/NeuralNet.hpp"
#include "impl/Activations.hpp"
#include "impl/Losses.hpp"

namespace {
double percentile(const std::vector<double>& sorted, double quantile) {
  const auto index = static_cast<std::size_t>(std::ceil(quantile * sorted.size())) - 1;
  return sorted[std::min(index, sorted.size() - 1)];
}
}  // namespace

int main() {
  constexpr std::size_t kWarmup = 1'000;
  constexpr std::size_t kIterations = 20'000;
  nn::NeuralNet network(std::make_unique<nn::MeanSquaredError>());
  network.add_dense(32, 64, std::make_shared<nn::Relu>(), 7);
  network.add_dense(64, 16, std::make_shared<nn::Relu>(), 11);
  network.add_dense(16, 4, std::make_shared<nn::Sigmoid>(), 13);
  nn::Vector input(32, 0.25);
  for (std::size_t index = 0; index < kWarmup; ++index) static_cast<void>(network.predict(input));
  std::vector<double> latency_us;
  latency_us.reserve(kIterations);
  const auto overall_start = std::chrono::steady_clock::now();
  for (std::size_t index = 0; index < kIterations; ++index) {
    const auto start = std::chrono::steady_clock::now();
    static_cast<void>(network.predict(input));
    const auto end = std::chrono::steady_clock::now();
    latency_us.push_back(std::chrono::duration<double, std::micro>(end - start).count());
  }
  const double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - overall_start).count();
  std::sort(latency_us.begin(), latency_us.end());
  const double mean = std::accumulate(latency_us.begin(), latency_us.end(), 0.0) / latency_us.size();
  std::cout << std::fixed << std::setprecision(3)
            << "{\n  \"iterations\": " << kIterations
            << ",\n  \"warmup_iterations\": " << kWarmup
            << ",\n  \"parameters\": " << network.parameter_count()
            << ",\n  \"latency_us\": {\"min\": " << latency_us.front()
            << ", \"mean\": " << mean << ", \"median\": " << percentile(latency_us, 0.50)
            << ", \"p95\": " << percentile(latency_us, 0.95)
            << ", \"p99\": " << percentile(latency_us, 0.99)
            << ", \"max\": " << latency_us.back()
            << "},\n  \"inferences_per_second\": " << kIterations / elapsed << "\n}\n";
  return 0;
}
