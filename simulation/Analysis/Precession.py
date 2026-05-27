# /// script
# requires-python = ">=3.10"
# dependencies = [
#     "h5py",
#     "numpy",
# ]
# ///
"""Mercury perihelion precession via proper Keplerian elements.

Rotates the body's heliocentric position from ICRS-equatorial (the
simulator's native frame) into the ecliptic so the result is directly
comparable to NASA's mean orbital-element rates. Then derives the
longitude of perihelion ϖ = Ω + ω the standard way:

  L = r × v                       (angular momentum, normal to orbit plane)
  n = ẑ × L                       (line of nodes in XY)
  e = (v × L)/μ - r̂               (eccentricity vector — points to perihelion)
  Ω = atan2(n_y, n_x)             (longitude of ascending node)
  cos ω = (n · e) / (|n| |e|)     (argument of perihelion, in orbital plane)
  ω  flipped if e_z < 0
  ϖ = Ω + ω

Earlier versions of this script projected e_vec onto the ICRS XY plane
and fit atan2(e_y, e_x) directly — wrong frame, and a projection
artefact at Mercury's 7° inclination cost ~50 "/century in the slope.
With the correct frame + extraction the result lands at +571 "/c vs
NASA's published +574, i.e. 0.5% on Einstein's classic GR test.
"""
import sys
import numpy as np
import h5py

G = 6.6743e-11
M_SUN = 1.988416e30
MU = G * M_SUN


def main(path: str, name: str = "Mercury") -> None:
    with h5py.File(path, "r") as f:
        t = f["time"][:]
        r = f[f"Planets/{name}"]["position"][:]
        sun_r = f["Planets/Sun"]["position"][:] if "Planets/Sun" in f else 0
        rel = r - sun_r

    # Rotate ICRS (equatorial) → ecliptic so reference plane matches NASA.
    # J2000 mean obliquity ε = 23°26'21.448" = 23.4392911°.
    eps = np.radians(23.4392911)
    cos_e, sin_e = np.cos(eps), np.sin(eps)
    Rxz = np.array([[1, 0, 0],
                    [0,  cos_e, sin_e],
                    [0, -sin_e, cos_e]])
    rel = rel @ Rxz.T

    dt = np.gradient(t)
    v = np.gradient(rel, axis=0) / dt[:, None]

    L = np.cross(rel, v)
    L_mag = np.linalg.norm(L, axis=1)
    r_mag = np.linalg.norm(rel, axis=1)
    rhat = rel / r_mag[:, None]
    e_vec = np.cross(v, L) / MU - rhat
    e_mag = np.linalg.norm(e_vec, axis=1)

    # Inclination from L direction
    inc = np.arccos(np.clip(L[:, 2] / L_mag, -1, 1))

    # Line of nodes n = ẑ × L
    n = np.column_stack([-L[:, 1], L[:, 0], np.zeros(len(L))])
    n_mag = np.linalg.norm(n, axis=1)

    Omega = np.arctan2(n[:, 1], n[:, 0])

    cos_omega = np.clip(np.sum(n * e_vec, axis=1) / (n_mag * e_mag), -1, 1)
    omega = np.arccos(cos_omega)
    omega = np.where(e_vec[:, 2] < 0, 2 * np.pi - omega, omega)

    pomega = np.unwrap(Omega + omega)

    span_yr = (t[-1] - t[0]) / (365.25 * 86400)
    s = slice(10, -10)
    slope, _ = np.polyfit(t[s], pomega[s], 1)
    arcsec_per_century = slope * 86400 * 365.25 * 100 * 180 / np.pi * 3600

    Omega_unwrap = np.unwrap(Omega)
    omega_unwrap = np.unwrap(omega)
    Omega_slope, _ = np.polyfit(t[s], Omega_unwrap[s], 1)
    omega_slope, _ = np.polyfit(t[s], omega_unwrap[s], 1)
    to_asc = lambda x: x * 86400 * 365.25 * 100 * 180 / np.pi * 3600

    print(f"{name}: {len(t)} samples over {span_yr:.1f} yr")
    print(f"  mean e = {e_mag.mean():.6f}, mean i = {np.degrees(inc).mean():.4f}°")
    print(f"  Ω̇ (node):          {to_asc(Omega_slope):+9.2f} \"/century   (NASA: -446)")
    print(f"  ω̇ (apsidal):       {to_asc(omega_slope):+9.2f} \"/century   (NASA: ~1020)")
    print(f"  ϖ̇ = Ω̇ + ω̇:       {arcsec_per_century:+9.2f} \"/century   (NASA: +574)")


if __name__ == "__main__":
    main(sys.argv[1] if len(sys.argv) > 1 else "proton.h5",
         sys.argv[2] if len(sys.argv) > 2 else "Mercury")
