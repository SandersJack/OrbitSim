# OrbitSim

A C++ solar-system n-body simulator. Propagates the eight planets, the
Moon, and user-defined satellites under Newtonian gravity, and writes
the trajectories to HDF5 for analysis or to JSON for the bundled
Three.js viewer.

## Build

Requirements:
- CMake ≥ 3.20
- A C++17 compiler
- HDF5 (`brew install hdf5` on macOS, `apt install libhdf5-dev` on Debian/Ubuntu)

HighFive is fetched automatically by CMake; no other system deps.

```
cmake -S simulation -B simulation/build
cmake --build simulation/build -j
```

The executable lands at `simulation/build/OrbitSim`.

## Run

From `simulation/`:

```
./build/OrbitSim                 # default: 1-day step, 1000 days
./build/OrbitSim -Ts 3600 -St 30 # 1-hour step, 30 days
./build/OrbitSim -web            # JSON output for the web viewer
./build/OrbitSim -h              # full flag list
```

Flags:
- `-Ts <seconds>` integration time step (default 86400)
- `-St <days>` total simulated time (default 1000)
- `-SatList <path>` custom satellite config
- `-web` write `proton.json` for the web viewer instead of `proton.h5`

## Config

Three plain-text files under `simulation/config/`:
- `PlanetsList.dat` — name, position (AU), velocity (m/s) per body
- `MoonsList.dat`   — `Moon(Planet)` per line
- `SatelliteList.dat` — `Satellite(Name,Body) mass[kg] orbitHeight[km] dV[m/s] launchLong[deg] launchTime[day]`

Lines starting with `#` are comments.

## Output

HDF5 layout (`proton.h5`):

```
/time                              shape (N,)
/Planets/<Name>/position           shape (N, 3), metres
/Planets/<Name>/velocity           shape (N, 3), m/s
/Moons/<Name>/position             ...
/Satellites/<Name>/position        ...
```

## Analysis

A minimal trajectory plot is in `simulation/Analysis/Checking.py`. It
uses PEP 723 inline metadata so `uv` handles the dependencies:

```
uv run simulation/Analysis/Checking.py
```

## Web viewer

```
./build/OrbitSim -web
open ../website/index.html
```

The viewer reads `proton.json` and renders the system in Three.js.
