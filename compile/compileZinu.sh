#!/bin/bash
make clean
make
./upload.sh
sudo minicom