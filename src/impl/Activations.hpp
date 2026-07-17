#pragma once

#include "core/Activation.hpp"

namespace nn {

class Sigmoid final : public Activation {
 public:
  [[nodiscard]] double forward(double value) const noexcept override;
  [[nodiscard]] double derivative_from_output(double output) const noexcept override;
  [[nodiscard]] std::string_view name() const noexcept override { return "sigmoid"; }
};

class Relu final : public Activation {
 public:
  [[nodiscard]] double forward(double value) const noexcept override;
  [[nodiscard]] double derivative_from_output(double output) const noexcept override;
  [[nodiscard]] std::string_view name() const noexcept override { return "relu"; }
};

class Tanh final : public Activation {
 public:
  [[nodiscard]] double forward(double value) const noexcept override;
  [[nodiscard]] double derivative_from_output(double output) const noexcept override;
  [[nodiscard]] std::string_view name() const noexcept override { return "tanh"; }
};

}  // namespace nn
