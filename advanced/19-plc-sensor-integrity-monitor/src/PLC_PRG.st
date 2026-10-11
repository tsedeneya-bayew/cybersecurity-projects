PROGRAM PLC_PRG
VAR
    SensorLevel : REAL := 50.0;
    ReportedLevel : REAL;
    PumpOn : BOOL := FALSE;
    SensorLevelRaw : WORD := 5000;
    ReportedLevelRaw : WORD;
    PumpStatusRaw : WORD;
    AttackEnable : BOOL := FALSE;
END_VAR

(* Raw register value 5000 represents 50.00%. *)
SensorLevel := WORD_TO_REAL(SensorLevelRaw) / 100.0;

(* Controlled false-report injection for the local simulation. *)
IF AttackEnable THEN
    ReportedLevel := SensorLevel + 10.0;
ELSE
    ReportedLevel := SensorLevel;
END_IF;

(* Pump uses the incoming sensor level, with hysteresis. *)
IF SensorLevel < 40.0 THEN
    PumpOn := TRUE;
ELSIF SensorLevel > 60.0 THEN
    PumpOn := FALSE;
END_IF;

ReportedLevelRaw := REAL_TO_WORD(ReportedLevel * 100.0);
IF PumpOn THEN
    PumpStatusRaw := 1;
ELSE
    PumpStatusRaw := 0;
END_IF;
