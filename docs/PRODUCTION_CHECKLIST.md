# Production checklist

- [ ] Main is protected and requires CI, static-analysis, and security workflows.
- [ ] Release commit passes GCC/Clang, tests, 90% coverage, sanitizers, and Valgrind.
- [ ] CodeQL, Trivy, Gitleaks, and SBOM jobs pass with reviewed artifacts.
- [ ] Benchmark is compared with equivalent historical runs and material changes are explained.
- [ ] Package and non-root container smoke tests pass.
- [ ] Version, changelog, license, and release notes are correct.
- [ ] Rollback artifact is retained and its checksum recorded.
- [ ] If external data is accepted, size/schema limits and provenance are reviewed.
- [ ] If serialization is added, corruption, versioning, and untrusted-input tests pass.
- [ ] If networking is added, authentication, authorization, quotas, TLS, logging, and incident
      ownership are designed before deployment.

The current executable is a finite offline demo, not a service. Health checks, API availability,
database connectivity, rate limiting, and runtime secrets are therefore not applicable today.
