#pragma once

#include <memory>
#include <string_view>

namespace nn {

class Activation {
 public:
  virtual ~Activation() = default;
  [[nodiscard]] virtual double forward(double value) const noexcept = 0;
  [[nodiscard]] virtual double derivative_from_output(double output) const noexcept = 0;
  [[nodiscard]] virtual std::string_view name() const noexcept = 0;
};

using ActivationPtr = std::shared_ptr<const Activation>;

}  // namespace nn
