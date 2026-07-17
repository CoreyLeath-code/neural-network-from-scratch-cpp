# Benchmark report

## Methodology

`benchmark_network` constructs a deterministic 32-64-16-4 network, performs 1,000 untimed warmup
inferences, then records 20,000 individual steady-clock inference latencies. JSON output includes
parameter count, min, mean, median, P95, P99, max microseconds, and inferences per second.

CI stores the raw JSON as `benchmark-results`. It intentionally does not publish a hard-coded latency
value because shared-runner hardware and contention are not stable. Compare results only when compiler,
flags, CPU, operating system, topology, warmup, and iteration count match.

## Current baseline

The first green CI run from this implementation establishes the baseline. Download its artifact and
record the run URL, commit, compiler version, CPU, and JSON here before enabling regression failure
thresholds. Until a stable runner and sufficient history exist, the benchmark is observational.

## Not measured

Training speed, peak RSS, CPU utilization, GPU memory, GPU utilization, disk usage, multi-user load,
precision, recall, F1, ROC-AUC, MAP, MRR, and NDCG are not meaningful for this offline scalar XOR/demo
scope. They must not be fabricated. Add them only with a representative task and documented protocol.
