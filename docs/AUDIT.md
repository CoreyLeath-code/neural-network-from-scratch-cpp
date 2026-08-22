# Production-readiness audit

## Executive summary

This repository now contains a real C++17 feed-forward neural-network implementation with explicit
forward propagation, backpropagation, dense-layer updates, activation functions, loss calculation,
strict numeric CSV ingestion, deterministic tests, a reproducible inference benchmark, CMake/CPack
packaging, containerization, and CI/security controls.

**Release verdict:** **PASS WITH DOCUMENTED LIMITATIONS** for a versioned educational/research release,
provided the automated release-audit job succeeds. This is not a claim of production ML readiness,
high-performance tensor-library parity, or general model-quality validity.

The release process must follow this order:

1. audit the candidate on `main`;
2. build and test it;
3. enforce the coverage gate;
4. execute and validate the benchmark artifact;
5. build and smoke-test the container;
6. build the CPack archive;
7. only after those gates pass, create the immutable semantic-version tag;
8. publish the GitHub Release and GHCR package from that audited commit.

A `v1.0.0` tag was created before this ordering policy was introduced. It is treated as immutable and
will not be moved. The next audited release must use a new semantic version.

## Audited surfaces

| Surface | Evidence reviewed | Assessment |
| --- | --- | --- |
| Build system | `CMakeLists.txt` | C++17 library, executable, benchmark, tests, install rules, CPack |
| Correctness tests | `tests/test_network.cpp` | deterministic activations/layers, validation, overflow safety, XOR regression, CSV failure modes |
| CI | `.github/workflows/ci.yml` | GCC/Clang, tests, ASan/UBSan, Valgrind, coverage, package and container smoke tests |
| Security | `.github/workflows/security.yml`, CodeQL | CodeQL, blocking Trivy scan, Gitleaks, CycloneDX SBOM |
| Benchmark | `benchmarks/benchmark_network.cpp`, benchmark report | 1,000 warmups + 20,000 timed deterministic inference calls with JSON output |
| Container | `Dockerfile` | multi-stage Ubuntu 26.04 build/runtime, non-root UID 10001 |
| Release | `.github/workflows/release.yml` | audit-before-tag gate, immutable tag, CPack checksum, GHCR provenance/SBOM |
| Documentation | `README.md`, `docs/` | strong scope boundaries; minor README drift remains documented below |

## Strengths after remediation

- Clear separation between interfaces, implementations, application, tests, and benchmarks.
- RAII ownership; no owning raw pointers, manual allocation, or hidden global mutable state.
- Deterministic initialization and regression data.
- Validation of topology, vector sizes, finite values, learning rates, CSV shape, and row limits.
- Overflowing parameter updates are rejected without mutating the current parameters.
- GCC/Clang compiler matrix and warnings-as-errors.
- ASan/UBSan and Valgrind memory-safety gates.
- Enforced 90% line-coverage threshold.
- Static analysis, CodeQL, secret scanning, blocking Trivy scan, and SBOM generation.
- Reproducible benchmark shape and machine-readable output without fabricated portable latency claims.
- CPack release artifact and a non-root container runtime.

## Release blockers found in this audit

The following release-process issue is being remediated before the next tag:

- **Audit order:** the earlier workflow could create a tag before performing the release-specific audit.
  The next workflow revision makes the audit job a hard dependency of tag creation.

Documentation observations that do not invalidate the implementation but should be cleaned up:

- The README contains a mojibake em-dash sequence (`â€”`) in the introductory paragraph.
- The README says the image builds on Ubuntu 24.04 while the current Dockerfile uses Ubuntu 26.04.

These documentation defects must not be interpreted as runtime capabilities or benchmark evidence.

## Remaining weaknesses and technical debt

| Priority | Risk | Impact | Recommended next step |
| --- | --- | --- | --- |
| P0 | Scalar online training only | Poor throughput on realistic datasets | Establish matrix/batch benchmarks before selecting a kernel |
| P0 | No serialization format | Models cannot be safely persisted or promoted | Design a versioned, bounded format with corruption tests |
| P1 | MSE-only objective | Unsuitable for robust multi-class work | Add stable softmax/cross-entropy plus numerical gradient checks |
| P1 | Forward cache makes a network non-reentrant | Unsafe for concurrent inference/training on one instance | Separate immutable parameters from per-request workspace |
| P1 | CSV lacks header/schema/normalization semantics | Data misuse can remain numerically valid | Add an explicit dataset schema and preprocessing contract |
| P2 | No SIMD/BLAS/GPU | Limited scale | Profile representative shapes before optimizing |
| P2 | No protected release environment/signing policy | Maintainers can publish after CI gates | Add environment approval and artifact signing when operationally useful |

## Risk analysis

- **Correctness:** deterministic XOR is a regression signal, not proof of general gradient correctness.
  Numerical finite-difference tests should be expanded before adding optimizers or losses.
- **Security:** the runtime has no network or authentication surface. Primary threats are malicious CSV
  resource consumption, supply-chain compromise, unsafe future serialization, and memory errors.
- **Reliability:** exceptions fail closed, but there is no checkpoint/recovery mechanism for training.
- **Performance:** benchmark output is reproducible in shape and methodology, not portable in value.
- **Scalability:** memory grows with parameters plus cached activations; computation is scalar O(weights)
  per sample and intentionally single-threaded.

## Release evidence contract

A release is eligible for tagging only when the release workflow produces all of the following from the
same `main` commit:

- successful CMake Release build;
- passing GoogleTest/CTest suite;
- valid benchmark JSON from the deterministic 32-64-16-4 workload;
- line coverage at or above 90%;
- successful Docker build and runtime smoke test;
- successful CPack `.tar.gz` archive;
- SHA-256 checksum for the release archive.

The tag is created only after these checks pass. The GitHub Release and GHCR image must then resolve to
the exact audited commit. GHCR publication includes version/latest/SHA tags plus provenance and SBOM
attestations.

## Prioritized plan

1. Keep the audit-before-tag release gate mandatory.
2. Add finite-difference gradient tests over weights and multiple layer shapes.
3. Define serialization and dataset contracts before accepting external model/data files.
4. Add batch abstractions, measure allocation/copy profiles, then optimize only verified bottlenecks.
5. Add signed provenance and a maintained benchmark-history comparison after stable runners exist.
