#!/bin/zsh
# Start the sim in Docker and open the map in Chrome on the host

# Open the map in Chrome on the host machine
open -a "Google Chrome" "$(pwd)/data/map.html"

# Start the sim container (interactive, so you can type commands)
docker compose run --rm hla-sim
