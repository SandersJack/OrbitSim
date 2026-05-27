# /// script
# requires-python = ">=3.10"
# dependencies = [
#     "skyfield",
# ]
# ///
"""Fetch barycentric state vectors for the Sun + 8 planets at a given epoch
and write them to simulation/config/PlanetsList.dat.

Usage:
    uv run simulation/scripts/getCelestialPos.py [YYYY-MM-DD]

Defaults to today's date. Coordinates are barycentric (ICRS); the simulator
runs in this same frame so positions/velocities are consumed verbatim.
"""
import sys
from datetime import date

from skyfield.api import load


def main(epoch: str | None) -> None:
    ts = load.timescale()
    if epoch is None:
        d = date.today()
    else:
        y, m, d_ = map(int, epoch.split("-"))
        d = date(y, m, d_)
    t = ts.utc(d.year, d.month, d.day, 12, 0, 0)

    # DE441 covers 1550-2650; DE421 covers 1900-2050 — both fine for our use.
    eph = load("de421.bsp")

    bodies = [
        ("Sun",     "SUN"),
        ("Mercury", "MERCURY BARYCENTER"),
        ("Venus",   "VENUS BARYCENTER"),
        ("Earth",   "EARTH BARYCENTER"),
        ("Mars",    "MARS BARYCENTER"),
        ("Jupiter", "JUPITER BARYCENTER"),
        ("Saturn",  "SATURN BARYCENTER"),
        ("Uranus",  "URANUS BARYCENTER"),
        ("Neptune", "NEPTUNE BARYCENTER"),
    ]

    iso = t.utc_iso()
    lines = [
        f"# List of bodies for barycentric integration at epoch {iso}",
        "# name PosX[Au] PosY[Au] PosZ[Au] VelX[m/s] VelY[m/s] VelZ[m/s]",
    ]
    for name, key in bodies:
        state = eph[key].at(t)
        x, y, z = state.position.au
        vx, vy, vz = state.velocity.m_per_s
        lines.append(f"{name} {x:.17e} {y:.17e} {z:.17e} {vx:.17e} {vy:.17e} {vz:.17e}")

    out = "simulation/config/PlanetsList.dat"
    with open(out, "w") as f:
        f.write("\n".join(lines) + "\n")
    print(f"Wrote {len(bodies)} bodies to {out} at epoch {iso}")


if __name__ == "__main__":
    main(sys.argv[1] if len(sys.argv) > 1 else None)
