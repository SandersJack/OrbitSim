# /// script
# requires-python = ">=3.10"
# dependencies = [
#     "h5py",
#     "matplotlib",
# ]
# ///
import h5py
import matplotlib.pyplot as plt

file = h5py.File("proton.h5", "r")

fig = plt.figure(figsize=(10, 10))

for group in ("Planets", "Moons", "Satellites"):
    if group not in file:
        continue
    for name, body in file[group].items():
        pos = body["position"][:]
        plt.scatter(pos[:, 0], pos[:, 1], s=1, label=name)

plt.scatter(0, 0, c="orange", marker="*", label="Sun")
plt.legend(markerscale=4)
plt.axis("equal")
plt.show()
