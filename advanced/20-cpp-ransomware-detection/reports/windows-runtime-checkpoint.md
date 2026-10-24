# Windows runtime checkpoint — October 24, 2026

User-provided terminal evidence was reviewed. Personal account paths are omitted from this report. Screenshot publication remains pending.

## Observed on Windows

- Debug compilation completed and produced monitor.exe, simulator.exe and test_detector.exe.
- Running test_detector.exe printed PASS for detector thresholds, distinct counts, expiry, cooldown, escalation, canaries, entropy, extension correlation, JSON escaping, queue overflow/drain/concurrency, and invalid inputs.
- Simulator initialization completed successfully.
- Monitor reported BASELINE READY files=42 backend=polling, with a 250 ms polling interval.
- Normal simulator activity completed. The partial event log contains a modified event for sample_0.txt at elapsed_seconds=85.848605. Its sampled entropy changed from 3.923231 to 4.027235. The incident log was empty.
- During the subsequent burst attempt, the monitor failed to sample sample_23.txt and reported Monitoring incomplete. This run does not demonstrate successful burst detection or a complete run summary.

## Change and validation

A concurrent rename or temporary sharing conflict is a plausible cause of the read failure; the screenshot alone does not establish the exact operating-system error.

The collector now retries an entire interrupted snapshot, up to five total attempts with 25 ms delays. Partial snapshots are discarded, so unreadable entries are not silently reported as deletions. Retries emit warnings. Persistent errors still stop the run and leave partial evidence.

After this change, Linux compilation, detector unit tests and existing subprocess integration tests passed: normal activity produced 1 event and 0 incidents; burst activity produced 61 events and 3 incidents. Sandbox and report refusal checks also passed. These are Linux results, not Windows revalidation.

## Windows revalidation — successful guided run

The user pulled through commit e00a162, rebuilt Debug and reran test_detector.exe successfully. Fresh synthetic data was initialized and monitored with a 250 ms interval in reports-live-02.

One interrupted scan warning was observed for sample_2.txt; the retry recovered and monitoring continued. Three high-severity incidents were recorded:

| Incident | Score | Signals |
|---|---:|---|
| 1 | 4 | canary_changed |
| 2 | 6 | many_distinct_files_changed, canary_changed |
| 3 | 8 | many_distinct_files_changed, canary_changed, demo_extension_delete_create_pairs |

The final summary displayed in the user's terminal was:

```json
{"status":"complete","backend":"polling","events":62,"incidents":3,"interrupted":true}
```

The interrupted flag records the requested Ctrl+C shutdown. Status complete means the monitor drained its event queue and wrote its final summary; this is distinct from the earlier incomplete run.

This validates the guided Windows Debug build, core unit tests, synthetic burst detection, recovery from one interrupted scan, JSON incident inspection and orderly shutdown. It does not establish Windows Python integration-suite execution, general detection performance or production readiness. Original JSONL files have not been uploaded; observations here are transcribed from user-provided terminal evidence.

## Pending

Publish redacted screenshot evidence. Evaluate additional workloads and Windows-specific edge cases.
