from skyfield.api import load, Topos

def generate_top_of_file(startTime):
    return f"# List of Planets to be simulated at start time: {startTime} \n# name PosX[Au] PosY[Au] PosZ[Au] VelX[m/s] VelY[m/s] VelZ[m/s]\n"

# Set the date for which you want to calculate the position and velocity
date = "2024-00-01T12:00:00"

# Load the ephemeris data
eph = load('de421.bsp')

# Set the time
ts = load.timescale()
time = ts.utc(2024, 1, 1, 12, 0, 0)

planets = ["MERCURY BARYCENTER", "VENUS BARYCENTER", "EARTH BARYCENTER", "MARS BARYCENTER", "JUPITER BARYCENTER", "SATURN BARYCENTER", "URANUS BARYCENTER", "NEPTUNE BARYCENTER"]
names = ["Mercury","Venus","Earth","Mars","Jupiter","Saturn","Uranus","Neptune"]

lines = []

for i in range(len(planets)):
    body = eph[planets[i]]
    state = eph[planets[i]].at(time)
    position = state.position.au
    velocity = state.velocity.m_per_s
    lines.append(f"{names[i]} {position[0]} {position[1]} {position[2]} {velocity[0]} {velocity[1]} {velocity[2]}")
    
file_path = "config/PlanetsList.dat"

with open(file_path, "w") as file:
        file.write(generate_top_of_file(date))
        for line in lines:
            file.write(f"{line}\n")