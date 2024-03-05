DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
DIR=$(dirname "$(dirname "${DIR}")")

export ORBITSOURCE="${DIR}/simulation"
alias ORBITS="${ORBITSOURCE}/build/OrbitSim"

source ~/root/bin/thisroot.sh

source ../.venv/bin/activate