# /// script
# requires-python = ">=3.10"
# dependencies = [
#     "astroquery",
#     "astropy",
# ]
# ///
"""Fetch barycentric state vectors for a list of asteroids/minor bodies from
JPL Horizons and write them to simulation/config/AsteroidsList.dat.

Usage:
    uv run simulation/scripts/getAsteroidPos.py [YYYY-MM-DD]

The list of bodies + masses + radii is defined inline at the top of the
script so anyone can drop in more entries by adding a row. Horizons IDs
follow the JPL convention (use "1;" "2;" etc. for numbered asteroids;
the trailing ";" tells Horizons to treat as a small-body designation
rather than a major-body record).
"""
import sys
from datetime import date, timedelta

from astroquery.jplhorizons import Horizons

# (display name, Horizons ID, mass [kg], mean radius [m])
BODIES = [
    ("Ceres",   "1;",  9.3835e20, 469700.0),
    ("Pallas",  "2;",  2.040e20,  256000.0),
    ("Vesta",   "4;",  2.5908e20, 262700.0),
    ("Hygiea",  "10;", 8.32e19,   215000.0),
    ("Davida",  "511;",3.84e19,   163000.0),
    ("Eunomia", "15;", 3.12e19,   116000.0),
    ("Psyche",  "16;", 2.27e19,   113000.0),
    ("Interamnia","704;", 3.79e19, 153000.0),
]


def main(epoch: str | None) -> None:
    if epoch is None:
        d = date.today()
    else:
        y, m, dd = map(int, epoch.split("-"))
        d = date(y, m, dd)
    # Horizons wants start + stop + step; ask for a single sample at noon UTC.
    start = d.isoformat() + " 12:00"
    stop  = (d + timedelta(days=1)).isoformat() + " 12:00"

    lines = [
        f"# Minor bodies (barycentric ICRS state vectors at {start})",
        "# name mass[kg] radius[m] PosX[AU] PosY[AU] PosZ[AU] VelX[m/s] VelY[m/s] VelZ[m/s]",
    ]
    for name, hid, mass, radius in BODIES:
        # location="@0" is the solar system barycenter.
        obj = Horizons(id=hid, location="@0",
                       epochs={"start": start, "stop": stop, "step": "1d"})
        v = obj.vectors(refplane="earth")  # ICRS / J2000 ecliptic-aligned
        row = v[0]
        x, y, z = float(row["x"]), float(row["y"]), float(row["z"])
        # Horizons gives velocity in AU/day; convert to m/s.
        AU = 1.49597870700e11
        DAY = 86400.0
        vx = float(row["vx"]) * AU / DAY
        vy = float(row["vy"]) * AU / DAY
        vz = float(row["vz"]) * AU / DAY
        lines.append(
            f"{name} {mass:.6e} {radius:.6e} "
            f"{x:.17e} {y:.17e} {z:.17e} {vx:.17e} {vy:.17e} {vz:.17e}"
        )
        print(f"  {name:10s}  r={ (x*x+y*y+z*z)**0.5:.4f} AU")

    out = "simulation/config/AsteroidsList.dat"
    with open(out, "w") as f:
        f.write("\n".join(lines) + "\n")
    print(f"Wrote {len(BODIES)} minor bodies to {out}")


if __name__ == "__main__":
    main(sys.argv[1] if len(sys.argv) > 1 else None)
