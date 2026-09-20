# Engineering Record

This page records the board work. It keeps changes, reasons, evidence, results, and open questions together.

The record uses four simple labels:

- **Observed**: seen in a file, image, or direct test.
- **Reported**: written in the project notes, but the raw test log is not in this repository.
- **Open**: the test or work is not complete.
- **Not recorded**: this repository has no evidence for it yet.

## Record rules

For each new entry, write:

1. date and revision;
2. change;
3. reason;
4. evidence;
5. result;
6. next check.

Keep observations separate from guesses. Keep a design target separate from a measured result. Use one Git commit for one related change when possible.

## Project baseline

### 2026-09-20 — v3 source package

**Change**

Saved the EasyEDA Pro project archive in the repository:

[`hardware/easyeda/esp32s3-devkit-v3.epro2`](../hardware/easyeda/esp32s3-devkit-v3.epro2)

**Reason**

The native project file is needed for later schematic and PCB changes. PDF and Gerber files alone cannot be used as the main editable source.

**Observed**

- The archive has one schematic document and one PCB document.
- It has embedded device and footprint data.
- It has nine WebP image assets.
- The archive metadata names EasyEDA Pro editor version `3.2.203`.
- The archive passes a local ZIP integrity check.
- SHA-256: `87307733b983e868c155da1e48b15ecf7174de0ee3033b91b34138fb87811d07`.

**Not recorded**

- EasyEDA Pro GUI import was not checked in this environment.
- No fresh ERC or DRC report is in this repository.
- No oscilloscope capture, continuity log, or host USB log is in this repository.

## Revision history

### v1 — two-layer Eagle board

**Change**

The first board used a two-layer Autodesk Eagle layout. The 5 V and 3.3 V paths used about 6–8 mil traces.

**Reason for the next change**

The project notes report brownout and reset problems during radio activity. The narrow power paths were treated as a possible source of transient voltage drop.

**Record result**

The v1 board was not kept as the current design.

### v2 — EasyEDA board

**Change**

The design moved to EasyEDA. The project notes report these problems:

- USB D+ and D- were crossed.
- The LD1117 regulator had less input margin.
- The EN network used 10 kΩ and 100 nF, about 1 ms nominal RC time.
- The antenna keep-out did not cover all board layers.

**Record result**

The project notes report repeated USB connect/disconnect and enumeration failure.

### v3 — four-layer EasyEDA board

**Change**

The v3 layout uses:

- four layers;
- about 1.6 mm finished board thickness;
- JLC04161H-7628 stackup;
- 1 oz outer copper and 0.5 oz inner copper;
- ME6118A33B3G 3.3 V LDO;
- 10 kΩ and 1 µF EN network, about 10 ms nominal RC time;
- GPIO19 to USB D-;
- GPIO20 to USB D+;
- 8 mil USB trace width;
- 6 mil USB pair spacing;
- no more than 10 mil recorded intra-pair mismatch;
- antenna copper clearance on all four layers.

**Record result**

The project record reports a stable 3.30 V rail, native USB Serial/JTAG enumeration, and GPIO21 LED operation on the assembled v3 board.

These are reported results. This repository does not contain the raw measurement files yet.

## Layout decisions

### Power

**Change**

The v3 design uses wider power paths, local ceramic capacitors, and a larger bulk capacitor on the 3.3 V rail.

**Reason**

ESP32-S3 radio activity can create fast current changes. A board can pass an idle check and still have a short rail drop during radio activity.

**Next check**

Measure 3V3 at the ESP32 module pins during reset and Wi-Fi activity. Save the scope capture with the board revision and test setup.

### USB

**Change**

The recorded v3 mapping is:

```text
GPIO19 -> USB D-
GPIO20 -> USB D+
```

The pair is routed over the L2 ground reference.

**Reason**

The v2 record reports a crossed pair. The v3 layout keeps the pair together and keeps the reference plane under it.

**Design target**

The route uses 8 mil width and 6 mil spacing to target 90 Ω differential impedance on the selected stackup.

**Limit**

The 90 Ω value is a design target. It is not a TDR measurement in this repository.

### Reset and boot

**Change**

The EN network uses a 10 kΩ pull-up and a 1 µF capacitor. GPIO0 is used for BOOT control.

**Reason**

The v3 record uses a longer nominal RC time than v2 to give the 3.3 V rail more time to settle.

**Next check**

Check EN during power-up. Check that holding GPIO0 low before reset enters download mode.

### RF keep-out

**Change**

The antenna area under the ESP32-S3 module is cleared on Top, L2, L3, and Bottom.

**Reason**

Copper below a module antenna can change the RF environment. The keep-out must cover the full board stack.

**Next check**

Run the Wi-Fi scan on the assembled board. Record the scan result, reset count, and test power source.

## Bring-up record

| Test | Status | Evidence needed to close it |
|---|---|---|
| 5 V to GND short check | Open | Resistance value and board photo or test note |
| 3V3 to GND short check | Open | Resistance value after capacitors settle |
| USB D- to GPIO19 | Open | Continuity measurement |
| USB D+ to GPIO20 | Open | Continuity measurement |
| D+ and D- not shorted | Open | Continuity measurement |
| 3.30 V at module pins | Reported | Saved meter or scope record |
| 3.3 V during radio activity | Open | Scope capture at the module pins |
| EN reset timing | Open | Scope capture or clear test note |
| BOOT download mode | Open | Host log or test note |
| Native USB enumeration | Reported | Saved host log with board revision |
| GPIO21 LED | Reported | Firmware output or test photo |
| Wi-Fi scan | Open | Serial log and reset count |
| ERC | Not recorded | Fresh EasyEDA Pro ERC report |
| DRC | Not recorded | Fresh EasyEDA Pro DRC report |
| Fabrication readiness | Open | Source, checks, export review, and physical fit evidence |

## Files for this record

- [EasyEDA Pro source](../hardware/easyeda/esp32s3-devkit-v3.epro2)
- [Schematic PDF](../hardware/schematic/esp32s3-v3-schematic.pdf)
- [BOM](../hardware/bom/esp32s3-v3-bom.xlsx)
- [Gerber archive](../hardware/gerber/esp32s3-v3-gerbers.zip)
- [STEP model](../hardware/mechanical/esp32s3-v3.step)
- [PCB layout image](../hardware/renders/esp32s3-v3-pcb-layout.png)
- [3D render](../hardware/renders/esp32s3-v3-render.png)
- [Arduino test sketch](../firmware/arduino/blink_and_scan/blink_and_scan.ino)

## Why this record has this shape

The root README is the short entry page. This file holds the longer history.

The method follows two simple ideas:

- Keep the native source with the project. Keep PDF, Gerber, BOM, and photos as easy-to-view helper files.
- For each change, write what changed, why, what supports it, what happened, and what is still open.

The first idea follows the [OSHWA open hardware definition](https://oshwa.org/definition/) and its [sharing best practices](https://oshwa.org/resources/sharing-best-practices/). The second idea fits GitHub's use of commits to show who changed what and why. See [GitHub README guidance](https://docs.github.com/en/repositories/managing-your-repositorys-settings-and-features/customizing-your-repository/about-readmes) and [GitHub commit guidance](https://docs.github.com/en/pull-requests/reference/commits).

## New entry template

Copy this block for the next hardware change:

```markdown
## YYYY-MM-DD — revision or test name

**Change**

What changed?

**Reason**

Why was it changed?

**Evidence**

Which file, measurement, photo, or log supports the entry?

**Result**

What happened? Use `reported`, `checked`, `open`, or `failed` when useful.

**Next check**

What is still unknown or what should happen next?
```
