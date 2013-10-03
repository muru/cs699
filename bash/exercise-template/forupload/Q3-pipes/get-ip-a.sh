#!/bin/bash

# This script should print the IP address of your linux machine's eth0
# interface.  The output should only be an IP address (remove the
# irrelevant parts of the ifconfig command output).

ifconfig eth0 | grep -Eo 'addr:([0-9]+\.){3}[0-9]+' | cut -d":" -f 2
