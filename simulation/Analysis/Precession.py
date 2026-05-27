# /// script
# requires-python = ">=3.10"
# dependencies = [
#     "h5py",
#     "numpy",
# ]
# ///
"""Planet perihelion precession measured from the simulation.

Uses the Laplace-Runge-Lenz (eccentricity) vector
  e = (v × (r × v)) / μ - r̂
which points to perihelion at every timestep, so the longitude of perihelion
ϖ = atan2(e_y, e_x) is a smooth function we can fit directly. Far less noisy
than detecting perihelion passages from r-minima.

Reference rates for Mercury:
  Newtonian planet perturbations:   ~531 "/century
  General relativity contribution:   ~43 "/century
  Observed total:                   ~574 "/century

Run after a long sim. Velocity is computed by central-difference on
positions; for fine dt this is essentially the simulator's own velocity.
"""
import sys
import numpy as np
import h5py

G = 6.6743e-11
M_SUN = 1.98892e30
MU = G * M_SUN


def main(path: str, name: str = "Mercury") -> None:
    with h5py.File(path, "r") as f:
        t = f["time"][:]
        r = f[f"Planets/{name}"]["position"][:]
        sun_r = f["Planets/Sun"]["position"][:] if "Planets/Sun" in f else 0
        rel = r - sun_r

    # Central-difference velocity from positions.
    dt = np.gradient(t)
    v = np.gradient(rel, axis=0) / dt[:, None]

    # Specific angular momentum L = r × v.
    L = np.cross(rel, v)

    # Eccentricity vector e = (v × L)/μ - r̂.
    rhat = rel / np.linalg.norm(rel, axis=1, keepdims=True)
    e_vec = np.cross(v, L) / MU - rhat
    e_mag = np.linalg.norm(e_vec, axis=1)

    # Longitude of perihelion in the orbital plane (≈ ecliptic for our setup).
    pomega = np.unwrap(np.arctan2(e_vec[:, 1], e_vec[:, 0]))

    span_yr = (t[-1] - t[0]) / (365.25 * 86400)
    print(f"{name}: {len(t)} samples over {span_yr:.2f} yr")
    print(f"  mean eccentricity:       {e_mag.mean():.6f}")

    # Discard a few samples at each end (gradient edge artefacts).
    s = slice(5, -5)
    slope, _ = np.polyfit(t[s], pomega[s], 1)
    deg_per_century = slope * 86400 * 365.25 * 100 * 180 / np.pi
    arcsec_per_century = deg_per_century * 3600
    print(f"  perihelion precession: {arcsec_per_century:+.2f} \"/century"
          f"  ({deg_per_century:+.5f} °/century)")
    if name == "Mercury":
        print("  Reference: Newtonian ~531, GR ~43, observed ~574 \"/century")


if __name__ == "__main__":
    main(sys.argv[1] if len(sys.argv) > 1 else "proton.h5",
         sys.argv[2] if len(sys.argv) > 2 else "Mercury")
