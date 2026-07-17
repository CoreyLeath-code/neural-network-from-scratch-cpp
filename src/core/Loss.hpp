#pragma once

#include "core/Layer.hpp"

namespace nn {

class Loss {
 public:
  virtual ~Loss() = default;
  [[nodiscard]] virtual double value(const Vector& prediction, const Vector& target) const = 0;
  [[nodiscard]] virtual Vector gradient(const Vector& prediction, const Vector& target) const = 0;
};

}  // namespace nn
