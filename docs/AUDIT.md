# Production-readiness audit

## Executive summary

The original repository contained an elaborate build and delivery narrative but every C++ interface,
implementation, test, demo, and benchmark file was empty. CMake mixed application and test sources,
Docker copied a nonexistent binary, several workflows used invalid source roots, coverage had no
threshold, and Trivy/clang-tidy findings were converted into success. This change establishes a real,
small implementation and makes its verification claims enforceable.

## Strengths after remediation

- Clear separation between interfaces, implementations, application, tests, and benchmarks.
- RAII ownership; no owning raw pointers, manual allocation, or hidden global mutable state.
- Deterministic initialization and regression data.
- Validation of topology, vector sizes, finite values, learning rates, CSV shape, and row limits.
- Compiler matrix, warnings-as-errors, sanitizers, Valgrind, 90% coverage, static analysis, CodeQL,
  secret scanning, SBOM generation, CPack, and non-root container smoke test.

## Remaining weaknesses and technical debt

| Priority | Risk | Impact | Recommended next step |
| --- | --- | --- | --- |
| P0 | Scalar online training only | Poor throughput on realistic datasets | Establish matrix/batch benchmarks before selecting a kernel |
| P0 | No serialization format | Models cannot be safely persisted or promoted | Design a versioned, bounded format with corruption tests |
| P1 | MSE-only objective | Unsuitable for robust multi-class work | Add stable softmax/cross-entropy plus numerical gradient checks |
| P1 | Forward cache makes a network non-reentrant | Unsafe for concurrent inference/training on one instance | Separate immutable parameters from per-request workspace |
| P1 | CSV lacks header/schema/normalization semantics | Data misuse can remain numerically valid | Add an explicit dataset schema and preprocessing contract |
| P2 | No SIMD/BLAS/GPU | Limited scale | Profile representative shapes before optimizing |
| P2 | Release workflow is tag-driven without environment approval | Maintainers can publish directly | Add protected environments and provenance signing |

## Risk analysis

- **Correctness:** deterministic XOR is a regression signal, not proof of general gradient correctness.
  Numerical finite-difference tests should be expanded before adding optimizers or losses.
- **Security:** the runtime has no network or authentication surface. Primary threats are malicious CSV
  resource consumption, supply-chain compromise, unsafe future serialization, and memory errors.
- **Reliability:** exceptions fail closed, but there is no checkpoint/recovery mechanism for training.
- **Performance:** benchmark output is reproducible in shape and methodology, not portable in value.
- **Scalability:** memory grows with parameters plus cached activations; computation is scalar O(weights)
  per sample and intentionally single-threaded.

## Prioritized plan

1. Keep every current CI gate required on protected `main`.
2. Add finite-difference gradient tests over weights and multiple layer shapes.
3. Define serialization and dataset contracts before accepting external model/data files.
4. Add batch abstractions, measure allocation/copy profiles, then optimize only verified bottlenecks.
5. Add signed provenance and a maintained benchmark-history comparison after stable runners exist.
