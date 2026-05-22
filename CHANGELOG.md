# Changelog

## [Unreleased]

To do:
- decrease impedance of Ipeak measurement divider, remove buffer and feed signal directly to MCU
- rework power input (use DCDC modules instead of ACDC; do not use linear for large Vdrop)
- increase testpoints hole diameters; add extra "GND testpoints"
- add reset pull up
- decrease R324-R327 (total ~100 k for the path)
- increase R323, R328, R329 (total ~ 1 Meg)
- increase R330 and R331 (total 220 k)
- add 100 k in parallel to D303

## [1.0.0] - 2026-04-11

### Migrated existing project to template (KDT)

[Unreleased]: https://github.com/FLRHW/Strobe_main/compare/1.0.0...HEAD

[1.0.0]: https://github.com/FLRHW/Strobe_main/compare/7dbe69f50b9ce31433772c6b542f035e6a8d0f9c...1.0.0
