# Neural Network from Scratch in C++

[![CI](https://github.com/CoreyLeath-code/neural-network-from-scratch-cpp/actions/workflows/ci.yml/badge.svg)](https://github.com/CoreyLeath-code/neural-network-from-scratch-cpp/actions/workflows/ci.yml)
[![Static Analysis](https://github.com/CoreyLeath-code/neural-network-from-scratch-cpp/actions/workflows/static-analysis.yml/badge.svg)](https://github.com/CoreyLeath-code/neural-network-from-scratch-cpp/actions/workflows/static-analysis.yml)
[![Security](https://github.com/CoreyLeath-code/neural-network-from-scratch-cpp/actions/workflows/security.yml/badge.svg)](https://github.com/CoreyLeath-code/neural-network-from-scratch-cpp/actions/workflows/security.yml)
![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus)
![Coverage policy](https://img.shields.io/badge/coverage-90%25%20required-brightgreen)
![Sanitizers](https://img.shields.io/badge/ASan%20%7C%20UBSan-required-success)
![License](https://img.shields.io/github/license/CoreyLeath-code/neural-network-from-scratch-cpp)

A compact feed-forward neural network implemented with the C++17 standard libraryâ€”no tensor or ML
framework. The project keeps forward propagation, backpropagation, gradient descent, initialization,
loss, validation, and memory ownership visible while applying production-style build and verification
controls.

## Implemented scope

- Dense layers with deterministic Xavier-uniform initialization
- Numerically stable sigmoid, ReLU, and tanh activations
- Mean squared error with analytic gradient
- Online gradient descent and configurable connected topology
- Strict, bounded numeric CSV ingestion
- XOR end-to-end learning regression
- Reproducible inference benchmark with min, mean, median, P95, P99, max, and throughput
- GCC/Clang builds, GoogleTest, 90% coverage gate, ASan/UBSan, Valgrind, CodeQL, and container smoke test

This is a research and educational implementation, not a replacement for an optimized tensor library.
It does not provide batching, autodiff graphs, GPU execution, model serialization, or distributed
training.

## Architecture

```mermaid
flowchart LR
    X["Validated input vector"] --> D1["Dense + activation"]
    D1 --> DN["Additional dense layers"]
    DN --> Y["Prediction"]
    Y --> L["MSE loss"]
    L --> G["Loss gradient"]
    G --> B["Reverse layer traversal"]
    B --> U["Weight and bias update"]
```

`NeuralNet` owns layers with `std::unique_ptr`; activations are immutable shared strategies. Each
`DenseLayer` stores a flattened row-major weight matrix and the most recent forward input/output for
backpropagation. Backward propagation calculates the input gradient before mutating weights.

See [architecture](docs/architecture.md), [audit](docs/AUDIT.md), and
[ADR 0001](docs/adr/0001-ci-and-deployment-hygiene.md).

## Build and test

Requirements: CMake 3.16+, a C++17 compiler, and GoogleTest when `BUILD_TESTING=ON`.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
./build/neural_network
./build/benchmark_network
```

### Sanitizers and coverage

```bash
cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=Debug -DNN_ENABLE_SANITIZERS=ON
cmake --build build-asan --parallel
ctest --test-dir build-asan --output-on-failure

cmake -S . -B build-coverage -DCMAKE_BUILD_TYPE=Debug -DNN_ENABLE_COVERAGE=ON
cmake --build build-coverage --parallel
ctest --test-dir build-coverage --output-on-failure
```

CI rejects warnings, test failures, sanitizer findings, Valgrind errors, coverage below 90%, static
analysis findings, blocking vulnerabilities, secret findings, packaging failures, and container
startup failures. Scanner reports, coverage data, benchmark JSON, packages, and SBOMs are artifacts.

## API example

```cpp
#include <memory>

#include "core/NeuralNet.hpp"
#include "impl/Activations.hpp"
#include "impl/Losses.hpp"

nn::NeuralNet network(std::make_unique<nn::MeanSquaredError>());
network.add_dense(2, 4, std::make_shared<nn::Tanh>(), 7);
network.add_dense(4, 1, std::make_shared<nn::Sigmoid>(), 11);

const double loss = network.train_sample({0.0, 1.0}, {1.0}, 0.25);
const nn::Vector prediction = network.predict({0.0, 1.0});
```

Inputs, targets, gradients, learning rates, dimensions, topology, and numeric values are validated.
Invalid states throw `std::invalid_argument` or `std::logic_error` rather than producing undefined
behavior or silently corrupting a model.

## Benchmark contract

The benchmark performs 1,000 warmups followed by 20,000 timed inference calls on a deterministic
32-64-16-4 network. It emits machine-readable JSON. Results are hardware-, compiler-, and build-flag
specific and are not compared across unlike runners. See [benchmark methodology](docs/benchmarks.md)
and [benchmark report](benchmarks/benchmark_report.md).

## Container and releases

```bash
docker build -t neuralnet-cpp:local .
docker run --rm neuralnet-cpp:local
```

The multi-stage image compiles on Ubuntu 24.04 and runs as UID 10001. Tags matching `vMAJOR.MINOR.PATCH`
produce a CPack archive, GitHub Release, and GHCR image. Releases should be cut only from a green main
branch after benchmark and audit review; there is no long-running service or network health endpoint.

## Engineering guarantees and limitations

| Area | Guarantee | Limitation |
| --- | --- | --- |
| Reproducibility | Explicit seeds and fixed XOR ordering | Floating-point results vary slightly by platform |
| Memory safety | RAII plus sanitizer and Valgrind gates | No formal verification |
| Numerical safety | Stable sigmoid and finite-value checks | No mixed precision or overflow policy for every topology |
| Performance | Allocation-aware flattened weights and reproducible timing | Scalar loops; no SIMD, BLAS, threading, or GPU |
| Data safety | Row/column limits and strict numeric parsing | CSV only; no schema metadata or normalization |

## Repository map

```text
src/core/       Stable interfaces and shared vector type
src/impl/       Activations, loss, dense layer, network, CSV loader
tests/          Unit, negative, boundary, and XOR regression tests
benchmarks/     Reproducible inference harness and report
docs/           Architecture, audit, performance, and operational decisions
.github/        CI, static analysis, security, release, and Dependabot
```

## Contributing and security

Changes should include deterministic tests, preserve warnings-as-errors, and document numerical or
performance tradeoffs. Run tests and sanitizers before opening a pull request. See
[SECURITY.md](SECURITY.md) for private vulnerability reporting and supported-version policy.

## Roadmap

1. Mini-batch training and shuffle policies with deterministic seeds
2. Softmax and cross-entropy with numerical-gradient tests
3. Model serialization with versioning and corruption limits
4. Matrix kernels benchmarked before SIMD, BLAS, or parallel execution
5. Benchmark history comparison with statistically meaningful regression thresholds

Roadmap items are directional, not implemented capability claims.

## License

[MIT](LICENSE). The software is provided for education and research without fitness guarantees.
