#include "impl/CSVLoader.hpp"

#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace nn {

Dataset load_csv(const std::filesystem::path& path, std::size_t feature_columns,
                 std::size_t target_columns, std::size_t max_rows) {
  if (feature_columns == 0 || target_columns == 0 || max_rows == 0) {
    throw std::invalid_argument("CSV dimensions and max_rows must be positive");
  }
  std::ifstream stream(path);
  if (!stream) throw std::runtime_error("unable to open CSV file");
  Dataset dataset;
  std::string line;
  const std::size_t expected = feature_columns + target_columns;
  while (std::getline(stream, line)) {
    if (dataset.features.size() == max_rows) throw std::runtime_error("CSV row limit exceeded");
    if (line.empty()) continue;
    std::stringstream row(line);
    std::string cell;
    Vector values;
    while (std::getline(row, cell, ',')) {
      std::size_t consumed = 0;
      double value = 0.0;
      try {
        value = std::stod(cell, &consumed);
      } catch (const std::exception&) {
        throw std::runtime_error("CSV contains a non-numeric field");
      }
      if (consumed != cell.size() || !std::isfinite(value)) {
        throw std::runtime_error("CSV contains an invalid numeric field");
      }
      values.push_back(value);
    }
    if (values.size() != expected) throw std::runtime_error("CSV column count mismatch");
    dataset.features.emplace_back(values.begin(), values.begin() + feature_columns);
    dataset.targets.emplace_back(values.begin() + feature_columns, values.end());
  }
  if (dataset.features.empty()) throw std::runtime_error("CSV contains no data rows");
  return dataset;
}

}  // namespace nn
