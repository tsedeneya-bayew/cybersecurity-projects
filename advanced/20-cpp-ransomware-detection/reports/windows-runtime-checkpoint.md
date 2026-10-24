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

## Pending

Rebuild Debug on Windows and repeat the demonstration with fresh synthetic data and a new report folder. Confirm Windows burst alerts and a complete summary before claiming Windows runtime validation complete.
