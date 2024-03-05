DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
DIR=$(dirname "$(dirname "${DIR}")")

export ORBITSOURCE="${DIR}/simulation"
alias ORBITS="${ORBITSOURCE}/build/OrbitSim"

alias make_clean="rm build/* -r"

alias make="cmake -Bbuild/ -S.; cd build; make; cd .."

source ~/root/bin/thisroot.sh

source ../.venv/bin/activate