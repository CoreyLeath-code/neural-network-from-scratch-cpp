#pragma once

#include "core/Loss.hpp"

namespace nn {

class MeanSquaredError final : public Loss {
 public:
  [[nodiscard]] double value(const Vector& prediction, const Vector& target) const override;
  [[nodiscard]] Vector gradient(const Vector& prediction, const Vector& target) const override;
};

}  // namespace nn
