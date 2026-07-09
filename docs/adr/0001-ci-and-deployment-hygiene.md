# ADR 0001: CI and Deployment Hygiene Baseline

## Status

Accepted

## Context

The repository is a C++ neural network implementation intended to demonstrate low-level AI engineering skill. To make the project credible as a portfolio-quality engineering artifact, the build must be reproducible, tested, inspected, packaged, and runnable in a container.

## Decision

Adopt a hygiene baseline with the following gates:

1. CMake build with explicit source root.
2. GoogleTest unit tests run through CTest.
3. GCC and Clang compiler matrix.
4. Warnings-as-errors for core library builds.
5. AddressSanitizer and UndefinedBehaviorSanitizer.
6. Valgrind memory safety checks.
7. Coverage artifact generation.
8. Static analysis with clang-format, clang-tidy, and cppcheck.
9. Security automation with CodeQL, Trivy, Gitleaks, Dependabot, and SBOM generation.
10. Multi-stage Docker image with a non-root runtime user.
11. Release packaging through CPack and GitHub Releases.

## Consequences

Positive:

- Build failures surface earlier.
- Tests become deterministic and recruiter-visible.
- Memory safety issues are caught automatically.
- The repository communicates senior engineering discipline.

Tradeoffs:

- CI runs are longer.
- Strict linting may require cleanup before feature work.
- Security scanners can occasionally produce false positives that require triage.
