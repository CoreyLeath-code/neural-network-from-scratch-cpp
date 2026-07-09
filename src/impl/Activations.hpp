#pragma once
#include "../core/Activation.hpp"
#include <cmath>

class ReLU : public Activation {
public:
    double activate(double x) override { return x > 0 ? x : 0; }
    // Convention: derivative at x=0 is 1.0 (matches major deep learning
    // frameworks)
    double derivative(double x) override { return x >= 0 ? 1.0 : 0.0; }
};

class Sigmoid : public Activation {
public:
    double activate(double x) override {
        return 1.0 / (1.0 + std::exp(-x));
    }
    double derivative(double x) override {
        double s = activate(x);
        return s * (1.0 - s);
    }
};

class Tanh : public Activation {
public:
    double activate(double x) override { return std::tanh(x); }
    double derivative(double x) override {
        double t = std::tanh(x);
        return 1.0 - t * t;
    }
};
