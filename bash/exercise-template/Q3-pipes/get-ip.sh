#!/bin/bash

# Write a SINGLE LINE bash command to get the IP address of your linux
# machine's eth0 interface.  The output should correspond to the
# relevant line in the output of the ifconfig command.

ifconfig eth0 | grep -E 'addr:([0-9]+\.){3}[0-9]+'
