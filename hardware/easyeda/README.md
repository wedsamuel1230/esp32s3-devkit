# EasyEDA Pro source

`esp32s3-devkit-v3.epro2` is the native EasyEDA Pro project archive for the v3 board.

## Open the source

Open the archive with EasyEDA Pro using its project import/open flow. Keep the archive as one unit in the GitHub repository. The archive is not intended to be edited as a collection of manually extracted files, and the internal `.epru` data should not be edited with a text editor.

The companion files in `hardware/` are readable exports and review evidence:

- [schematic PDF](../schematic/esp32s3-v3-schematic.pdf)
- [BOM](../bom/esp32s3-v3-bom.xlsx)
- [Gerber archive](../gerber/esp32s3-v3-gerbers.zip)
- [STEP model](../mechanical/esp32s3-v3.step)
- [PCB layout render](../renders/esp32s3-v3-pcb-layout.png)
- [assembled-board photo](../photos/esp32s3-v3-assembled.jpg)

## Source audit

The archive was inspected locally on 2026-09-20. Its top-level project metadata identifies EasyEDA Pro editor version `3.2.203`. The archive contains one schematic document, one PCB document, embedded device/footprint data, and nine WebP image assets. No password, API key, private-key marker, or similar credential string was found during the local archive scan.

This is a structural audit only. The project was not opened in the EasyEDA Pro editor in this environment, so successful interactive import, schematic-to-PCB parity, DRC, and fabrication readiness remain separate checks.

## Integrity

SHA-256:

```text
87307733b983e868c155da1e48b15ecf7174de0ee3033b91b34138fb87811d07  esp32s3-devkit-v3.epro2
```
