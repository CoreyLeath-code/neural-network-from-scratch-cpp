#pragma once

#include <cstddef>
#include <filesystem>
#include <vector>

#include "core/Layer.hpp"

namespace nn {

struct Dataset {
  std::vector<Vector> features;
  std::vector<Vector> targets;
};

[[nodiscard]] Dataset load_csv(const std::filesystem::path& path, std::size_t feature_columns,
                               std::size_t target_columns, std::size_t max_rows = 1'000'000);

}  // namespace nn
