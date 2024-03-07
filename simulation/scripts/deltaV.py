import numpy as np
import argparse

def dv(M, r1, r2):
    G = 6.67430e-11
    return np.sqrt((G * M) / r1) * (np.sqrt((2 * r2)/(r1 + r2)) - 1)

def main():
    
    m_sun = 1.989e30
    AU = 1.496e11
    
    parser = argparse.ArgumentParser(description='Description of your script.')
    
    parser.add_argument('r1', type=int, help='Semi-major axis of start in AU')
    parser.add_argument('r2', type=float, help='Semi-major axis of target in AU')

    # Parse the command-line arguments
    args = parser.parse_args()
    
    r1 = args.r1 * AU
    r2 = args.r2 * AU

    delta_v = dv(m_sun, r1, r2)
    print(f"Expected delta V {delta_v} m/s")
    
if __name__ == "__main__":
    main()