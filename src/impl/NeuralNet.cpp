#include "../core/NeuralNet.hpp"

NeuralNet::NeuralNet(std::unique_ptr<Loss> loss)
    : lossFn(std::move(loss)) {}

void NeuralNet::addLayer(std::unique_ptr<Layer> layer) {
    layers.push_back(std::move(layer));
}

std::vector<double> NeuralNet::forward(const std::vector<double>& input) {
    std::vector<double> current = input;
    for (auto& layer : layers) {
        current = layer->forward(current);
    }
    last_output = current;
    return last_output;
}

void NeuralNet::backward(const std::vector<double>& y_true, double lr) {
    std::vector<double> grad = lossFn->gradient(y_true, last_output);
    for (int i = static_cast<int>(layers.size()) - 1; i >= 0; --i) {
        grad = layers[i]->backward(grad, lr);
    }
}

double NeuralNet::train(const std::vector<double>& x,
                        const std::vector<double>& y, double lr) {
    forward(x);
    double loss = lossFn->compute(y, last_output);
    backward(y, lr);
    return loss;
}
