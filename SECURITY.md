# Security policy

## Supported versions

Security fixes are applied to the latest commit on `main`. No older release line is currently
maintained.

## Reporting

Use GitHub private vulnerability reporting for this repository. Do not open a public issue containing
exploit details, secrets, or sensitive inputs. Include the affected revision, platform, compiler,
sanitizer output, minimal reproduction, impact, and suggested mitigation when available.

## Threat model

The executable is an offline numerical demonstration. Relevant threats are malformed or oversized CSV
files, non-finite inputs, dimension confusion, memory-safety defects, compromised build dependencies,
secret leakage in repository history, and unsafe future model deserialization. Controls include bounded
strict parsing, runtime validation, RAII, ASan/UBSan, Valgrind, CodeQL, Trivy, Gitleaks, Dependabot, and
CycloneDX SBOM generation.

There is no HTTP API, authentication system, database, or production secret requirement. Those controls
must be designed before adding a network service rather than represented as current capabilities.
