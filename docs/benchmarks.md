# Benchmark guide

Build in Release mode and run `benchmark_network`. Preserve the emitted JSON together with commit SHA,
compiler and version, build flags, CPU model, operating system, and CI run URL. Warmup and measured
iteration counts are part of the benchmark contract.

Use median for typical latency, P95/P99 for tail behavior, and throughput only for the single-threaded
closed-loop harness. Minimum is sensitive to timer noise; maximum is sensitive to scheduler pauses.
Do not compare Debug, sanitizer, shared-runner, and dedicated-runner results.

Add a regression gate only after collecting enough results on stable hardware to quantify natural
variance. A threshold should include statistical headroom and link to the change that established it.
