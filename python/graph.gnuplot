set datafile separator ","
set zeroaxis
set yrange[0:150]
set xlabel "Time"
set ylabel "bpm"
set nokey

#set xdata time
#set timestamp "%s"
#set format x "%H:%M:%S"

# filename can be passed using gnuplot -e "filename='test.txt'" graph.gnuplot 

# set format x "%m/%d/%Y %H:%M:%S"
plot filename with lines smooth bezier

pause -1 "Hit return to continue"
