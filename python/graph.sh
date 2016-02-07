#!/bin/bash

filename="$1"
echo $filename
gnuplot -e "filename='${filename}'" graph.gnuplot


