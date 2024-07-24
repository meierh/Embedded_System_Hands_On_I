#!/bin/bash

RASPI_HOSTNAME="$1"
RASPI_USERNAME="$2"
SOURCE="Code-raspi/main.py"
source_file=$(basename "$SOURCE")

# Copy Python file to Raspberry Pi
scp "$SOURCE" "$RASPI_HOSTNAME:/home/$RASPI_USERNAME"

# Execute script
ssh -t "$RASPI_HOSTNAME" "source .venv/bin/activate && \
                          python3 /home/$RASPI_USERNAME/$source_file"