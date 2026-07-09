#pragma once
#include "../core/Layer.hpp"
#include <random>
#include <vector>

class DenseLayer : public Layer {
public:
    // seed=0 uses a non-deterministic (time-based) seed; any other value gives
    // a reproducible initialization (useful for testing and debugging).
    explicit DenseLayer(size_t in, size_t out, unsigned int seed = 0)
        : input_size(in), output_size(out) {
        std::mt19937 gen(seed == 0 ? std::random_device{}()
                                    : static_cast<std::mt19937::result_type>(seed));
        std::uniform_real_distribution<> dis(-1.0, 1.0);
        weights.resize(out, std::vector<double>(in));
        bias.resize(out);
        for (auto& row : weights)
            for (auto& w : row)
                w = dis(gen);
        for (auto& b : bias)
            b = dis(gen);
    }

    std::vector<double> forward(const std::vector<double>& input) override {
        last_input = input;
        std::vector<double> out(output_size, 0.0);
        for (size_t i = 0; i < output_size; ++i) {
            for (size_t j = 0; j < input_size; ++j)
                out[i] += weights[i][j] * input[j];
            out[i] += bias[i];
        }
        return out;
    }

    std::vector<double> backward(const std::vector<double>& grad,
                                 double lr) override {
        std::vector<double> grad_input(input_size, 0.0);
        for (size_t i = 0; i < output_size; ++i) {
            for (size_t j = 0; j < input_size; ++j) {
                grad_input[j] += grad[i] * weights[i][j];
                weights[i][j] -= lr * grad[i] * last_input[j];
            }
            bias[i] -= lr * grad[i];
        }
        return grad_input;
    }

private:
    size_t input_size, output_size;
    std::vector<std::vector<double>> weights;
    std::vector<double> bias;
    std::vector<double> last_input;
};
