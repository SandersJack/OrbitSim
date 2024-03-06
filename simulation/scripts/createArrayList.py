def generate_satellite(name, body, mass, orbit_height, delta_v, launch_longitude, launch_time):
    return f"Satellite({name},{body}) {mass} {orbit_height} {delta_v} {launch_longitude} {launch_time}"

def generate_top_of_file():
    return "# List of Satelites to be simulated \n# Satellite(Name,Body that it is in orbit around) Mass[kg] OrbitHeight[km] DeltaV[m/s] LaunchLongatude[Deg] LaunchTime[Day] \n"

def generate_satellites(num_satellites, mass, orbit_height, delta_v, launch_step_days):
    satellites = []
    for i in range(num_satellites):
        name = f"Satellite{i}"
        body = "Earth"  
        mass = mass  
        orbit_height = orbit_height 
        delta_v = delta_v 
        launch_longitude = -1
        launch_time = i * launch_step_days

        satellites.append(generate_satellite(name, body, mass, orbit_height, delta_v, launch_longitude, launch_time))

    return satellites

def append_satellites_to_file(file_path, satellites):
    with open(file_path, "w") as file:
        file.write(generate_top_of_file())
        for satellite in satellites:
            file.write(f"{satellite}\n")

def main():
    existing_file_path = "config/SatelliteList_array.txt"
    num_new_satellites = 10 
    mass = 10000
    orbit_height = 500
    delta_v = 2900
    launch_step_days = 100
    new_satellites = generate_satellites(num_new_satellites, mass, orbit_height, delta_v, launch_step_days)
    append_satellites_to_file(existing_file_path, new_satellites)

    print(f"{num_new_satellites} satellites added to {existing_file_path}.")

if __name__ == "__main__":
   main()