#pragma once
#include <vector>
#include <memory>
#include "Layer.hpp"
#include "Loss.hpp"

class NeuralNet {
public:
    explicit NeuralNet(std::unique_ptr<Loss> loss);

    void addLayer(std::unique_ptr<Layer> layer);
    std::vector<double> forward(const std::vector<double>& input);
    void backward(const std::vector<double>& y_true, double lr);
    double train(const std::vector<double>& x,
                 const std::vector<double>& y, double lr);

private:
    std::vector<std::unique_ptr<Layer>> layers;
    std::unique_ptr<Loss> lossFn;
    std::vector<double> last_output;
};
