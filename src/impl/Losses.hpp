#pragma once
#include "../core/Loss.hpp"
#include <cmath>
#include <stdexcept>

class MSE : public Loss {
public:
    double compute(const std::vector<double>& y,
                   const std::vector<double>& y_hat) override {
        double sum = 0.0;
        for (size_t i = 0; i < y.size(); ++i) {
            double d = y[i] - y_hat[i];
            sum += d * d;
        }
        return sum / static_cast<double>(y.size());
    }

    std::vector<double> gradient(const std::vector<double>& y,
                                 const std::vector<double>& y_hat) override {
        std::vector<double> g(y.size());
        for (size_t i = 0; i < y.size(); ++i)
            g[i] = -2.0 * (y[i] - y_hat[i]) / static_cast<double>(y.size());
        return g;
    }
};

class BinaryCrossEntropy : public Loss {
public:
    double compute(const std::vector<double>& y,
                   const std::vector<double>& y_hat) override {
        const double eps = 1e-9;
        double sum = 0.0;
        for (size_t i = 0; i < y.size(); ++i) {
            double p = std::max(eps, std::min(1.0 - eps, y_hat[i]));
            sum +=
                -y[i] * std::log(p) - (1.0 - y[i]) * std::log(1.0 - p);
        }
        return sum / static_cast<double>(y.size());
    }

    std::vector<double> gradient(const std::vector<double>& y,
                                 const std::vector<double>& y_hat) override {
        const double eps = 1e-9;
        std::vector<double> g(y.size());
        for (size_t i = 0; i < y.size(); ++i) {
            double p = std::max(eps, std::min(1.0 - eps, y_hat[i]));
            g[i] = (-y[i] / p + (1.0 - y[i]) / (1.0 - p)) /
                   static_cast<double>(y.size());
        }
        return g;
    }
};
