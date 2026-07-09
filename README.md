Neural Network From Scratch (C++) — Low-Level AI Engineering
# Neural Network From Scratch (C++)

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.16+-064F8C?logo=cmake)
![Linux](https://img.shields.io/badge/Linux-Ubuntu%2024.04-E95420?logo=ubuntu)
![Docker](https://img.shields.io/badge/Docker-Ready-2496ED?logo=docker)
![GoogleTest](https://img.shields.io/badge/Tested-GoogleTest-25A162)
![Coverage](https://img.shields.io/badge/Coverage-LCov-success)
[![Enterprise CI](https://github.com/CoreyLeath-code/neural-network-from-scratch-cpp/actions/workflows/ci.yml/badge.svg)](https://github.com/CoreyLeath-code/neural-network-from-scratch-cpp/actions/workflows/ci.yml)
[![Security](https://github.com/CoreyLeath-code/neural-network-from-scratch-cpp/actions/workflows/security.yml/badge.svg)](https://github.com/CoreyLeath-code/neural-network-from-scratch-cpp/actions/workflows/security.yml)
[![Static Analysis](https://github.com/CoreyLeath-code/neural-network-from-scratch-cpp/actions/workflows/static-analysis.yml/badge.svg)](https://github.com/CoreyLeath-code/neural-network-from-scratch-cpp/actions/workflows/static-analysis.yml)
[![Release](https://github.com/CoreyLeath-code/neural-network-from-scratch-cpp/actions/workflows/release.yml/badge.svg)](https://github.com/CoreyLeath-code/neural-network-from-scratch-cpp/actions/workflows/release.yml)
![CodeQL](https://img.shields.io/badge/CodeQL-Enabled-2F80ED?logo=github)
![Trivy](https://img.shields.io/badge/Trivy-Security%20Scanning-1904DA)
![Gitleaks](https://img.shields.io/badge/Gitleaks-Secrets%20Scanning-orange)
![SBOM](https://img.shields.io/badge/SBOM-CycloneDX-blue)
![Valgrind](https://img.shields.io/badge/Memory-Valgrind-success)
![ASan](https://img.shields.io/badge/ASan-Enabled-success)
![UBSan](https://img.shields.io/badge/UBSan-Enabled-success)
![clang-tidy](https://img.shields.io/badge/clang--tidy-Enabled-blue)
![cppcheck](https://img.shields.io/badge/cppcheck-Enabled-green)
![clang-format](https://img.shields.io/badge/clang--format-Enforced-blueviolet)
![License](https://img.shields.io/github/license/CoreyLeath-code/neural-network-from-scratch-cpp)
![Last Commit](https://img.shields.io/github/last-commit/CoreyLeath-code/neural-network-from-scratch-cpp)
![Repo Size](https://img.shields.io/github/repo-size/CoreyLeath-code/neural-network-from-scratch-cpp)
![Issues](https://img.shields.io/github/issues/CoreyLeath-code/neural-network-from-scratch-cpp)
![Pull Requests](https://img.shields.io/github/issues-pr/CoreyLeath-code/neural-network-from-scratch-cpp)


This project implements a fully connected feedforward neural network entirely from scratch in modern C++ (C++17) without external ML libraries.

The objective is to demonstrate:

Deep understanding of forward and backward propagation

Manual gradient descent optimization

Modular layer abstraction

Configurable network topology

Performance benchmarking

Clean production build systems using CMake

CI/CD pipeline validation

This repository emphasizes engineering rigor, not just algorithmic implementation.


/neural-network-from-scratch-cpp
├── CMakeLists.txt      # The "Brain" of your build
├── src/
│   ├── main.cpp
│   └── network.cpp
├── include/
│   └── network.h
└── tests/
    └── test_network.cpp

🏗 Architecture
Diagram is not supported.
Core Components

NeuralNetwork — Top-level network abstraction

Layer — Dense layer with weights and biases

Activation Functions — ReLU, Sigmoid, Tanh

Loss Functions — MSE / Cross-Entropy

Backpropagation Engine — Gradient computation

Training Loop — Epoch-based learning

Benchmarking Module — Performance measurement

📊 Performance Metrics
Metric	Value
Parameters (2-8-1 Network)	25
Forward Pass Time (1000 samples)	~1.8 ms
Training Time (1000 iterations)	~0.35 sec
XOR Classification Accuracy	100%
Memory Usage	Minimal (stack + std::vector)

Benchmarks collected on x86_64 Ubuntu using O2 optimization.

📈 Benchmark Example
Training time (1000 iterations): 0.342 seconds
Average Forward Pass Latency: 0.0018 ms

Benchmarking uses std::chrono for high-resolution timing.

🛠 Build Instructions
Requirements

C++17 compatible compiler

CMake ≥ 3.16

Build
git clone https://github.com/Trojan3877/neural-network-from-scratch-cpp.git
cd neural-network-from-scratch-cpp

cmake -S . -B build
cmake --build build
Run
./build/neural_net
Run Tests
./build/test_forward
⚡ Quick Start Example
NeuralNetwork nn({2, 8, 1});
nn.train_sample({0.5, 0.2}, {1.0});
auto output = nn.forward({0.5, 0.2});

Fully configurable architecture:

NeuralNetwork nn({input_size, hidden1, hidden2, output_size});
🧮 Mathematical Foundation

Forward propagation:

𝑧
=
𝑊
𝑥
+
𝑏
z=Wx+b
𝑎
=
𝑓
(
𝑧
)
a=f(z)

Loss (MSE):

𝐿
=
1
𝑛
∑
(
𝑦
−
𝑦
^
)
2
L=
n
1
	​

∑(y−
y
^
	​

)
2

Backpropagation uses chain rule:

∂
𝐿
∂
𝑊
=
∂
𝐿
∂
𝑎
⋅
∂
𝑎
∂
𝑧
⋅
∂
𝑧
∂
𝑊
∂W
∂L
	​

=
∂a
∂L
	​

⋅
∂z
∂a
	​

⋅
∂W
∂z
	​


Weight update:

𝑊
:
=
𝑊
−
𝛼
∇
𝑊
W:=W−α∇W
🔄 CI/CD

This repository includes:

GitHub Actions C++ build validation

Automated compilation checks

Multi-file build pipeline using CMake

CI Status Badge reflects current build health.

📁 Project Structure
include/
src/
tests/
benchmarks/
docs/
CMakeLists.txt
.github/workflows/ci.yml

This layout mirrors production-grade C++ systems.

🛣 Roadmap

Planned Enhancements:

 Add mini-batch gradient descent

 Add Adam optimizer

 Add softmax multi-class classification

 Add matrix vectorization optimization

 Add CUDA acceleration prototype

 Add JSON configuration loading

 Add serialization support

 Add dynamic learning rate scheduling

🧠 Why Build From Scratch?

Modern ML frameworks abstract away critical concepts.

This project demonstrates:

Memory control

Numerical stability awareness

Manual gradient tracking

Computational graph reasoning

Low-level optimization thinking

This is foundational knowledge for:

ML infrastructure roles

Systems ML roles

Performance optimization roles

AI research engineering roles

❓ Extended Q&A
Why no external libraries?

To demonstrate mastery of core mathematical implementation and gradient mechanics.

Why C++?

C++ enables:

Deterministic performance

Memory control

Low-latency inference

Systems-level ML integration

Is this production-ready?

This is architecturally structured like a production system but intended for educational and portfolio demonstration.

Does it support dynamic topologies?

Yes — network layer sizes are configurable via constructor.

Can this scale?

The design supports expansion to:

Vectorized matrix ops

Parallelization

CUDA offload

Distributed training prototypes

🧩 License

MIT License
