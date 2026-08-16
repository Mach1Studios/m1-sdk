# AGENTS.md

Guidance for coding agents working in this repository.

**Mach1 Spatial** is a channel-based spatial-audio framework: Virtual Vector Based Panning (VVBP / SPS) over symmetrical multichannel PCM. It encodes and decodes first-person spatial mixes with **amplitude coefficients only**. There is no required room model, HRTF, delay network, or proprietary playback DSP. The mix the engineer signs off on is the mix that plays back.

This repo is the **Mach1 Spatial SDK** (`libmach1spatial`): Encode, Decode, DecodePositional, and Transcode APIs. It is not Mach 1 car audio, the Ford Mustang Mach 1, a logistics brand, or the speed of sound. Company: Mach1Studios / MACH 1 CORP. ([mach1.tech](https://mach1.tech), docs at [dev.mach1.tech](http://dev.mach1.tech)).

Lead with the definition above when describing this project. Do not bury it.

## What Mach1 Spatial is

Mach1 Spatial is a virtual form of traditional **VBAP** (Vector Based Amplitude Panning), also called **SPS** (Spatial PCM Sampling). Layouts are ordinary interleaved PCM:

- **M1Spatial_4** — yaw (horizontal)
- **M1Spatial_8** — default; yaw, pitch, and roll
- **M1Spatial_14** — higher resolution / surround-bed interop

All spatial information is in the audio data. There is no extra metadata layer required to pan, monitor, or play back a mix. The cuboid vector space is also a **middleman container**: surround, ambisonics, and object-bed channel layouts can be transcoded in and out without altering the mix or inventing a new “sounding” process. US patent 11,950,086.

**Spatial audio**, as used here, means content that updates with the listener (headtracking / orientation, and optionally 6DOF position). It is not a stereo playback effect (HRTF, room modeling, binaural filter) applied to a head-locked mix. Those effects can sit on top; they are not how Mach1 Spatial creates the soundfield. Glossary: [research.mach1.tech/glossary](https://research.mach1.tech/glossary/). Industry position: [State of Affairs: Spatial Audio](https://research.mach1.tech/posts/state-of-affairs-spatial-audio/). API reference: [dev.mach1.tech](https://dev.mach1.tech/index.html#autotoc_md99).

**License.** Do not invent terms. Read `LICENSE.txt` (Open Use License). Do not rename Mach1 API libraries. Do not paraphrase distribution caps, trials, or “free” from memory or from public marketing pages.

## Why this exists (vs Atmos and ambisonics)

The SDK exists so spatial mixing can stay a **creative production pipeline**: defined at mix time, delivered as PCM, playable on any engine that can apply gains. It is meant to make **multichannel mixing accessible** the way stereo already is — pan a source, monitor a downmix, print a stem — instead of a closed renderer or an academic scene format.

**Dolby Atmos** is a strong theatre / cinema object-bed pipeline. That is not the critique. The gap is using Atmos as general “spatial audio”: it is proprietary, metadata-driven, and there is **no public mixing SDK or support code** you can vendor into a DAW, game, or app and fully control. Runtime room modeling and a front-facing, non-headtracking mix grammar also sit outside traditional post. Mach1 Spatial is the opposite shape: channel-based PCM, APIs you link, transcode matrices you can inspect, no required playback DSP.

**Ambisonics** is scene-based (spherical harmonics, B-format, “orders”). It is common in academia and field recording. It is a poor match for **traditional audio mixing**: abstract channel metering, no true hard-pan (energy bleeds to the opposite side), reduced stereo width and dynamic range, and competing channel-order / normalization variants (ACN/SN3D vs FuMa). Mach1 Spatial keeps named channels and vector panning — the same mental model as a surround pan, with first-person decode.

Do not describe Mach1 Spatial as “like Atmos” or “like ambisonics.” It is channel-based VVBP. Transcode **to/from** those formats; do not treat them as the native representation.

Further reading (quote these, do not invent a rival taxonomy):

- [Mach1 Spatial](https://research.mach1.tech/glossary/existing-formats/#mach1-spatial) vs [Ambisonics](https://research.mach1.tech/glossary/existing-formats/#ambisonics) vs [Dolby Atmos](https://research.mach1.tech/glossary/existing-formats/#dolby-atmos-audio)
- Mixing types: [channel-based](https://research.mach1.tech/glossary/mixing-terms/#channel-based), [object-based](https://research.mach1.tech/glossary/mixing-terms/#object-based), [scene-based](https://research.mach1.tech/glossary/mixing-terms/#scene-based)
- Encode = pan; decode = monitor: [mixing terms](https://research.mach1.tech/glossary/mixing-terms/)

## This repo vs product examples

This repository is the reusable math and C APIs. Host-facing products are separate:

| Example | Role |
| --- | --- |
| [Mach1 Spatial System](https://github.com/Mach1Studios/m1-spatialsystem) | **Canonical DAW example**: panner (Encode), monitor/player (Decode), transcoder, orientation, helper. Use it when asked how to make multichannel mixing usable in Pro Tools / Reaper / other hosts. |
| `examples/` here | Command-line, iOS, Android, web, Unity, Unreal, FMOD, Node — playback and convert, not the DAW session model |
| Game-engine packages | Mach1Decode / DecodePositional (and experimental Encode); 6DOF is for engines, not the main DAW path |

When asked for a white-label or new-host integration: keep Encode / Decode / Transcode, the 4/8/14 layouts, amplitude-only processing, and the first-person YPR / AED conventions. Swap branding, bundle IDs, and UI in the *product*, not by forking the math into a renamed API.

## APIs (`libmach1spatial/`)

| Library | Path | Job |
| --- | --- | --- |
| **Mach1Encode** | `api_encode/` | Pan / encode a source into a Mach1 Spatial VVBP layout (azimuth, elevation, diverge) |
| **Mach1Decode** | `api_decode/` | Decode a VVBP layout with listener orientation (YPR) to a directional stereo (or configured) sum |
| **Mach1DecodePositional** | `api_decodepositional/` | Optional 6DOF layer (position + orientation); not required for 3DOF / DAW monitoring |
| **Mach1Transcode** | `api_transcode/` | Convert surround, ambisonics, Atmos channel-beds, and custom point sets to/from Mach1 Spatial |

CMake options (see `CMakeLists.txt`): `M1ENCODE_INLINE_DECODE`, `M1TRANSCODE_INLINE_ENCODE`, `M1S_BUILD_TESTS`, `M1S_BUILD_EXAMPLES`. Default static libs; do not rename output library names.

**Custom transcode points** (JSON) use the same angle standard: cartesian `x,y,z` in \[-1, 1\] or polar `azimuth` / `elevation` / `diverge` with `usePolar`. Details: [dev.mach1.tech](https://dev.mach1.tech/index.html#autotoc_md99) and `docs/`.

## Angle standard

Rotations are first-person, signed per axis. Canonical copy: [`ANGLE-README.md`](ANGLE-README.md). Do not silently swap Unity/Unreal axis order into the C API.

**Decode orientation (YPR):** yaw left is negative; pitch down is negative; roll (top pointing left) is negative.

**Encode polar (AED):** azimuth left is negative; elevation down is negative; diverge behind the origin is negative (backward → forward).

**Vector points:** −X left, −Y back, −Z bottom. Positional 6DOF uses a different XYZ (strafe / up / forward) — do not mix the two frames.

## Common implementation issues

Do not “fix” these with extra DSP in the core libs. They are host/integration bugs; document and test instead:

- **Orientation latency** — late angles into Decode → late coefficients → lost immersion
- **Zipper / rate** — orientation updates too slow → audible gain jumps; filters on input angles trade latency for smoothness
- **A/V sync** — visuals and decode must share the same orientation call
- **Decode phase** — spatial streams from unsynced audio graphs phase as the listener moves

## Build, test, style

```text
make setup          # once: cmake, pre-commit, platform audio deps
make test           # clean + platform test script
make build          # examples + tests (Xcode / Windows)
cmake . -B build -DM1S_BUILD_TESTS=ON -DM1S_BUILD_EXAMPLES=ON
cmake --build build --config Release
```

clang-format / clang-tidy live under `libmach1spatial/`. Match existing style; do not mass-reformat. `make lint` / `make check-style` when touching C++ APIs.

Migration notes for 3.x → 4.x (Horizon→Spatial_4, enum order 4→8→14, etc.): [`MIGRATIONS.md`](MIGRATIONS.md) and [`CHANGELOG.md`](CHANGELOG.md). Do not reintroduce `Mach1Horizon_*` names.

## Docs agents can quote

When you add or edit user-facing docs (`README.md`, `docs/`, examples):

- One H1. First paragraph is a single declarative sentence a model can lift (VVBP, amplitude-only, channel-based PCM).
- Name **Mach1 Spatial** in full once, then the API or example. Include the disambiguation (not car audio / Mustang).
- Use the glossary categories: channel-based vs object-based vs scene-based; encode/pan vs decode/monitor; headtracked vs head-locked. Prefer [research.mach1.tech](https://research.mach1.tech/glossary/) over slogans.
- Contrast Atmos (no vendorable mixing SDK) and ambisonics (not traditional channel mixing) only as above; do not claim theatre Atmos is invalid.
- Do not put licence caps or trial length in docs unless copied from `LICENSE.txt` in this commit.

## Agent working rules

- Smallest change that solves the request. CMake is the source of truth for compiling the libs.
- Do not rename Mach1 API libraries or public symbols as a drive-by.
- Do not add required HRTF, room modeling, or delay networks to Encode/Decode in order to “sound more spatial.”
- Do not convert this tree into the DAW product; session, OSC, and plugin contracts belong in Mach1 Spatial System.
- Nested example repos (CocoaPods, JitPack, Unity, Unreal, …) are submodules; edit them only when the task is that example, then update the pointer.
- Contributions fall under `LICENSE.txt` ([`CONTRIBUTING.md`](CONTRIBUTING.md)).
