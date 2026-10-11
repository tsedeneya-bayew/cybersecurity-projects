# Findings — Project 19

**Author:** Tsedeneya Bayew  
**Evidence date:** October 11, 2026  
**Status:** In progress; initial integrity-detection milestone demonstrated.

## Objective

Compare a PLC-reported tank level with a Simulink plant reference obtained through a branch that bypasses the PLC, then test a controlled false-report offset.

## Actual observations

| Check | Observed result | Evidence |
|---|---|---|
| Manual Modbus write/read | 7000 → [7000, 0]; 3000 → [3000, 1] | Command outputs reported during the lab; no screenshot included |
| Baseline detector | Display 0 | Observed during the lab; screenshot omitted |
| False-report alert | Display 1 during active simulation | Observed during the lab; screenshot omitted |
| Recovery | Display 0 during active simulation after offset removed | Observed during the lab; screenshot omitted |
| Difference trace | Approximately 0.2–0.3 points normally; approximately 10 during injection; near baseline afterward | [Scope trace](../screenshots/04-mismatch-trace.png) |

The final Scope screenshot shows a high plateau at approximately 82–92 simulation seconds. Earlier Display screenshots show active and cleared states at 61.8 s and 85.5 s respectively. These do not provide a synchronized timeline for one event. No measured detection latency, false-positive rate, or detection accuracy is claimed.

## Interpretation

The prototype identified the deliberately introduced difference between the report and simulation reference. Removing the report bias restored agreement within the demonstrated threshold. A successful Modbus exchange alone did not guarantee that the reported process value was accurate.

This is a controlled PLC-program fault injection. It does not demonstrate unauthorized access, exploitation, or detection of an actual attacker.

## Issues resolved

- Modbus write input required double values, despite uint16 register precision.
- A rounding block was needed before writing fractional simulated levels to integer registers.
- Scope1 retained a manual scale suited to numerical noise, making the later trace appear blank. Auto Y-axis scaling made it visible.
- OPC UA connection attempts encountered certificate-usage errors; the tested demonstration proceeded with Modbus TCP.

## Open issues and limits

A startup spike exceeds the threshold. Startup handling and a persistence filter are planned. The captured run also warned that pacing could not maintain the requested rate. Simulation seconds are not a verified real-time measurement.

The independent reference is a Simulink signal. A physical system requires a separately trusted measurement path and health checks. A mismatch cannot independently attribute the problem to the sensor, PLC, communication path, or model.

Native project files and raw recorded samples are absent from the repository. Reproduction requires building and mapping the local model and PLC program as described in the guide.

## Cleanup / retention

The documented recovery step writes `AttackEnable = FALSE`; Display 0 was observed afterward. No automatic pump shutdown was added. The local lab is retained for further research. Stop Simulink when finished and verify AttackEnable is FALSE before a new baseline run.

## Next validation

Record the reference level, PLC report, pump status, mismatch, alert, and switch state with timestamps. Evaluate startup behavior, repeated fault on/off runs, sensor-path manipulation, and communication failures before making broader EDR claims.
