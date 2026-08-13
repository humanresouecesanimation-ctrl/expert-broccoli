# FluidLab 0.1

FluidLab is a standalone C++ 3D particle-fluid simulator with a professional dynamics-workstation interface. Its layout follows the familiar fluid-production workflow: scene tree, 3D viewport, node parameters, simulation transport, timeline, cache, meshing, and export controls. The solver uses Smoothed Particle Hydrodynamics (SPH), a spatial hash for neighbor lookup, container collisions, interactive emission, and PLY cache export.

## Windows build

Install Visual Studio 2022 with **Desktop development with C++**, CMake, and Git. Then open **Developer PowerShell for VS 2022**:

```powershell
cd FluidLab
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
.\build\Release\FluidLab.exe
```

CMake downloads raylib 5.5 during the first configuration. After that, the executable builds locally.

## Create the Windows installer

Install NSIS, then run `build-installer.ps1` from PowerShell. It produces
`dist/FluidLab-Setup.exe`, including Desktop and Start Menu shortcuts and an
uninstaller registered with Windows Installed Apps. See `BUILD-INSTALLER.txt`.

## Controls

| Key | Action |
|---|---|
| Space | Play or pause |
| R | Reset simulation |
| N | Advance one rendered frame |
| J | Inject a horizontal fluid jet |
| Up / Down | Change viscosity |
| E | Toggle PLY cache recording |
| Mouse wheel | Camera zoom |

The interface buttons also provide play/pause, stepping, resetting, timeline transport, and cache recording. Scene nodes can be selected from the left panel and their settings appear in the right panel.

Cache files are written to `cache/fluid_000000.ply` and include position and velocity per particle.

## Current scope

This is a foundation, not yet a RealFlow replacement. The next engineering milestones are GPU compute, signed-distance-field mesh collisions, marching-cubes surface generation, Alembic export, project files, and a node-based emitter/force UI.
