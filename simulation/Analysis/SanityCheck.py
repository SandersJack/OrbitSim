# /// script
# requires-python = ">=3.10"
# dependencies = [
#     "h5py",
#     "numpy",
# ]
# ///
"""Print per-body orbital ranges and counts from a simulation HDF5 file.

Usage:
    uv run simulation/Analysis/SanityCheck.py [path/to/proton.h5]

Default path is ./proton.h5. Used as a quick regression check after
physics changes — see whether each planet still sits in its expected
range and completes the right number of orbits in the simulated span.
"""
import sys
import numpy as np
import h5py

AU = 1.495e11

PLANET_ORDER = [
    "Mercury", "Venus", "Earth", "Mars",
    "Jupiter", "Saturn", "Uranus", "Neptune",
]


def orbit_count(xy: np.ndarray) -> float:
    """Half the number of y-sign-changes."""
    y = xy[:, 1]
    return float(np.sum(np.sign(y[1:]) != np.sign(y[:-1])) / 2)


def summarise(name: str, pos: np.ndarray, unit_scale: float, unit_label: str) -> None:
    r = np.linalg.norm(pos, axis=1) / unit_scale
    print(
        f"{name:<12} "
        f"init={r[0]:>9.4f} final={r[-1]:>9.4f} "
        f"min={r.min():>9.4f} max={r.max():>9.4f}  "
        f"orbits={orbit_count(pos):>5.1f}  [{unit_label}]"
    )


def main(path: str) -> None:
    with h5py.File(path, "r") as f:
        n_steps = f["time"].shape[0]
        days = (f["time"][-1] - f["time"][0]) / 86400 if n_steps > 1 else 0
        print(f"{path}: {n_steps} steps spanning {days:.0f} days\n")

        print("Planets (heliocentric):")
        for name in PLANET_ORDER:
            grp = f.get(f"Planets/{name}")
            if grp is None:
                continue
            summarise(name, grp["position"][:], AU, "AU")

        # Moon state is stored geocentric (relative to its parent), so distance
        # from origin already equals distance from parent body.
        if "Moons" in f:
            print("\nMoons (geocentric, distance from parent body):")
            for name, grp in f["Moons"].items():
                summarise(name, grp["position"][:], 1e3, "km")

        if "Satellites" in f:
            print("\nSatellites (heliocentric):")
            for name, grp in f["Satellites"].items():
                summarise(name, grp["position"][:], AU, "AU")


if __name__ == "__main__":
    main(sys.argv[1] if len(sys.argv) > 1 else "proton.h5")
