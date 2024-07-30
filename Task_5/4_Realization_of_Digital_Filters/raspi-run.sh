#!/bin/bash

if [[ $# != 2 ]]; then
  echo "USAGE: $0 <hostname> <username>"
  exit 1
fi

RASPI_HOSTNAME="$1"
RASPI_USERNAME="$2"
SOURCE="python-uart/main.py"
source_file=$(basename "$SOURCE")

# Copy Python file to Raspberry Pi
scp "$SOURCE" "$RASPI_HOSTNAME:/home/$RASPI_USERNAME" 1>&2

# Execute script
ssh -t "$RASPI_HOSTNAME" "source .venv/bin/activate && \
                          python3 /home/$RASPI_USERNAME/$source_file 5000 0" > measurements.csv