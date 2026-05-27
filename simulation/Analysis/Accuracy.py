# /// script
# requires-python = ">=3.10"
# dependencies = [
#     "h5py",
#     "numpy",
# ]
# ///
"""Per-planet orbital element accuracy vs published NASA values.

Computes semi-major axis (a), eccentricity (e), and orbital period from
the simulation output and reports % error against the canonical figures.
Run after a long sim (1 yr per orbit needed; ~100 yr at 1-day step is
adequate for inner planets, 200+ yr for outer).
"""
import sys
import numpy as np
import h5py

AU = 1.495978707e11
DAY = 86400.0
YR = 365.25 * DAY

# NASA fact-sheet values: (semi-major axis [AU], eccentricity, period [days]).
REF = {
    "Mercury": (0.38710, 0.20563, 87.97),
    "Venus":   (0.72333, 0.00677, 224.70),
    "Earth":   (1.00000, 0.01671, 365.25),
    "Mars":    (1.52366, 0.09339, 686.97),
    "Jupiter": (5.20336, 0.04839, 4332.59),
    "Saturn":  (9.53707, 0.05415, 10759.22),
    "Uranus":  (19.1913, 0.04727, 30688.50),
    "Neptune": (30.0690, 0.00859, 60182.00),
}


def derive_elements(times: np.ndarray, pos: np.ndarray):
    r = np.linalg.norm(pos, axis=1) / AU
    rmin, rmax = r.min(), r.max()
    a = 0.5 * (rmax + rmin)
    e = (rmax - rmin) / (rmax + rmin) if (rmax + rmin) > 0 else 0.0

    # Period via y-sign-changes: time between successive perihelions.
    y = pos[:, 1]
    crossings = np.where(np.sign(y[1:]) != np.sign(y[:-1]))[0]
    if len(crossings) >= 2:
        # Each full orbit gives 2 zero-crossings.
        period_days = (times[crossings[-1]] - times[crossings[0]]) / DAY / (len(crossings) - 1) * 2
    else:
        period_days = float("nan")

    return a, e, period_days


def pct(sim, ref):
    if ref == 0 or np.isnan(sim):
        return float("nan")
    return 100.0 * (sim - ref) / ref


def main(path: str) -> None:
    with h5py.File(path, "r") as f:
        times = f["time"][:]
        span_yr = (times[-1] - times[0]) / YR
        print(f"{path}: span {span_yr:.1f} yr, {len(times)} steps\n")

        # Heliocentric elements: subtract the Sun's barycentric position so
        # numbers compare directly to NASA's heliocentric fact-sheet values.
        sun = f.get("Planets/Sun")
        sun_pos = sun["position"][:] if sun is not None else None

        header = f"{'body':<10} {'a (AU)':>9} {'Δa %':>7} {'e':>8} {'Δe %':>9} {'T (d)':>9} {'ΔT %':>7}"
        print(header)
        print("-" * len(header))

        for name, (a_ref, e_ref, T_ref) in REF.items():
            grp = f.get(f"Planets/{name}")
            if grp is None:
                continue
            pos = grp["position"][:]
            if sun_pos is not None:
                pos = pos - sun_pos
            a, e, T = derive_elements(times, pos)
            print(
                f"{name:<10} "
                f"{a:>9.5f} {pct(a, a_ref):>+7.2f} "
                f"{e:>8.5f} {pct(e, e_ref):>+9.2f} "
                f"{T:>9.2f} {pct(T, T_ref):>+7.2f}"
            )


if __name__ == "__main__":
    main(sys.argv[1] if len(sys.argv) > 1 else "proton.h5")
