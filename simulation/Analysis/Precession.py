# /// script
# requires-python = ">=3.10"
# dependencies = [
#     "h5py",
#     "numpy",
# ]
# ///
"""Mercury perihelion precession measured from the simulation.

Computes the longitude of perihelion ϖ at each detected perihelion passage,
fits a linear trend, and reports arcsec/century. Reference values:
  Newtonian (planet perturbations only):   ~531 "/century
  General relativity contribution:          ~43 "/century
  Observed (total):                         ~574 "/century

Run after a long sim (≥100 yr at fine dt). Per-orbit perihelion samples are
noisy on 1-hr steps, so longer spans give cleaner trends.
"""
import sys
import numpy as np
import h5py


def main(path: str, name: str = "Mercury") -> None:
    with h5py.File(path, "r") as f:
        times = f["time"][:]
        body = f[f"Planets/{name}"]["position"][:]
        sun = f["Planets/Sun"]["position"][:] if "Planets/Sun" in f else 0
        rel = body - sun
        r = np.linalg.norm(rel, axis=1)

        # Detect perihelion passages: r reaches a local minimum.
        # Use a 3-point window to refine the minimum via a parabolic fit.
        perih_idxs = np.where((r[1:-1] < r[:-2]) & (r[1:-1] < r[2:]))[0] + 1
        perih_t = []
        perih_lon = []
        for i in perih_idxs:
            # Parabolic refinement on r(t).
            t0, t1, t2 = times[i-1], times[i], times[i+1]
            r0, r1, r2 = r[i-1], r[i], r[i+1]
            denom = (r0 - 2 * r1 + r2)
            if denom == 0:
                t_min = t1
                x_min, y_min = rel[i, 0], rel[i, 1]
            else:
                # Quadratic fit: assume uniform-ish spacing; minimum at offset.
                offset = 0.5 * (r0 - r2) / denom
                t_min = t1 + offset * (t2 - t1)
                # Refine x,y by linear interpolation around the same offset.
                x_min = rel[i, 0] + offset * 0.5 * (rel[i+1, 0] - rel[i-1, 0])
                y_min = rel[i, 1] + offset * 0.5 * (rel[i+1, 1] - rel[i-1, 1])
            perih_t.append(t_min)
            perih_lon.append(np.arctan2(y_min, x_min))

        perih_t = np.array(perih_t)
        perih_lon = np.unwrap(np.array(perih_lon))

        span_yr = (times[-1] - times[0]) / (365.25 * 86400)
        print(f"{name}: {len(perih_t)} perihelion passages over {span_yr:.1f} yr")

        if len(perih_t) < 3:
            print("Need more orbits to fit a precession trend.")
            return

        # Linear fit ϖ(t).
        slope, intercept = np.polyfit(perih_t, perih_lon, 1)  # rad / s
        deg_per_century = slope * 86400 * 365.25 * 100 * 180 / np.pi
        arcsec_per_century = deg_per_century * 3600
        print(f"Perihelion precession: {arcsec_per_century:+.1f} \"/century"
              f"  ({deg_per_century:+.4f} °/century)")
        print("  Reference: Newtonian ~531, GR ~43, observed ~574 \"/century")


if __name__ == "__main__":
    main(sys.argv[1] if len(sys.argv) > 1 else "proton.h5",
         sys.argv[2] if len(sys.argv) > 2 else "Mercury")
