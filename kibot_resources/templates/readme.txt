<p align="center" width="100%">
  <img alt="Logo" width="33%" src="Logos/dummy_logo.svg">
</p>

<h1 align="center">${BOARD_NAME}</h1>

<p align="center" width="100%">
  <a href="${GIT_URL}/actions/workflows/ci.yaml">
    <img alt="CI Badge" src="${GIT_URL}/actions/workflows/ci.yaml/badge.svg?branch=">
  </a>
</p>

<p align="center" width="100%">
    <img src="Images/dummy_image.png">
</p>

***

<p align="center">
  <img alt="3D Top Angled" src="${png_3d_viewer_angled_top_outpath}" width="45%">
&nbsp; &nbsp; &nbsp; &nbsp;
  <img alt="3D Bottom Angled" src="${png_3d_viewer_angled_bottom_outpath}" width="45%">
</p>

***


# **1 kV strobe - Intro**
Since my tenage years I spend a great chunk of my spare time on hobby electronics. Back then, much of that time was to build FM transmitters, audio amplifiers and strobe lights. I was always pushing my builds as much as I could, (i.e., extracting pressurised smoke from capacitors!) as an attempt to make the flashes ever brighter. It was somewhat challenging given the limited experience and resources I had at the time, but a lot of fun nonetheless.
Life went on, priorities changed and for a long time I did not "play" with strobe lights... until recently, after I bumped into an ebay page of a large XOP-15 bulb and, well, nostalgy happened.

While defining the project scope, I had to decide if I'd use the good old Xenon bulb or move to LEDs, as these have greatly evolved and are ubiquitous nowadays, making their way to commercial strobes (https://www.martin.com/en/products/atomic-3000-led). I have decided to stick to vintage tech, as in my mind this would perhaps bring more interesting power electronics challenges, since the bulb requires a somewhat high bus voltage and can draw very high peak currents when ionised.

So that's what this repo contains: a Xenon-based strobe light, aiming to drive a XOP-15 bulb with up to 1 kV, while controlling the duration and frequency of pulses.


## Project overview
This repository contains KiCad v9 design files and STM32 source code for the main board of a Xenon-based strobe light system.
The system is divided into three separate boards:
- HMI - so the user can set desired operating conditions (bus voltage, pulse width and pulse frequency)
- Main board - drives the Xenon bulb, generating the trigger pulse and actively interrupting the discharge according to the required pulse width
- Power supply - to produce an adjustable DC supply. To realise that I intend to eventually design an AC-mains powered converter, including a PFC circuit. As an interim solution, off-the-shelf boost converter modules are used: two such modules are connected in series, each providing up to 450 V output while supplied by 12 V batteries.

Working prototypes exist for both the HMI and main board. This repository covers only the main board and the HMI board will soon be available on its onw repo.

As a side project, the boost modules have been reverse engineered with the aim to modify them from non-isolated to isolated. This work is covered under the following repos:
- https://github.com/FLRHW/StepUp_module_12to450V
- https://github.com/FLRHW/Isolated_12to450V.

## Design targets
DC bus voltage: 400 V to 1000 V  
Flash rate: 0.1 Hz to 100 Hz  
Pulse duration: 20 μs to 10 ms  

Please note that extremes are not simultaneously achievable. This design relies on storing energy on a capacitor bank to then discharge it into the bulb, hence some time must be reserved for storing that energy. The pulse duration is currently limited in SW to 50 % of the period as a safeguard, but for practical use it must be a much lower percentage.

Also, the output capacitors currently in use are not large enough to reach 10 ms pulse duration - at least not at high output voltages as the bulb current seems to increase non linearly with the bus voltage.

## Intended use
This repository is intended for:
* Educational use and study of the circuit;
* Integration into hobby or research projects.

## License
The design files and documentation in this repository are released under the CERN Open Hardware Licence v2 – Permissive (CERN‑OHL‑P‑2.0). SPDX identifier: CERN-OHL-P-2.0 See the LICENSE file for the full legal text and conditions.
In short and non‑legal terms, you are free to use, modify, manufacture, and distribute hardware based on these files, provided you respect attribution requirements and the terms in the license.

## Trademarks and third‑party rights
All product names, logos, and brands eventually mentioned are the property of their respective owners and are not included or licensed here.
Any reference to third‑party marks is purely descriptive and does not imply endorsement or affiliation.

## Safety and disclaimer
* The author takes no responsibility for any injury, death or damage arising from the use, direct or indirect, of the design or otherwise any information from this repository
* You shall not build this or any prototype based on this design unless you are absolutely sure of your abilities dealing with this type of circuit
* High voltage can cause injury, death, or damage to equipment. Ensure appropriate protections and testing procedures are followed
* The design is provided as is, with no warranty of any kind
* Users are responsible for checking any patent, copyright, or regulatory constraints that may apply to their own use


***

## DIRECTORY STRUCTURE
    │
    ├─ Computations       # Misc calculations
    ├─ HTML               # HTML files for generated webpage
    ├─ Images             # Pictures and renders
    │
    ├─ kibot_resources    # External resources for KiBot
    │  ├─ colors          # Color theme for KiCad
    │  ├─ fonts           # Fonts used in the project
    │  ├─ scripts         # External scripts used with KiBot
    │  └─ templates       # Templates for KiBot generated reports
    │
    ├─ kibot_yaml         # KiBot YAML config files
    ├─ KiRI               # KiRI (PCB diff viewer) files
    │
    ├─ lib                # KiCad footprint and symbol libraries
    │  ├─ 3d_models       # Component 3D models
    │  ├─ lib_fp          # Footprint libraries
    │  └─ lib_sym         # Symbol libraries
    │
    ├─ Logos              # Logos
    │
    ├─ Manufacturing      # Assembly and fabrication documents
    │  ├─ Assembly        # Assembly documents (BoM, pos, notes)
    │  │
    │  └─ Fabrication     # Fabrication documents (ZIP, notes)
    │     ├─ Drill Tables # CSV drill tables
    │     └─ Gerbers      # Gerbers
    │
    ├─ Report             # Reports for ERC/DRC
    ├─ Schematic          # PDF of schematic
    ├─ Templates          # Title block templates
    ├─ Testing
    │  └─ Testpoints      # Testpoints tables      
    │
    └─ Variants           # Outputs for assembly variants
