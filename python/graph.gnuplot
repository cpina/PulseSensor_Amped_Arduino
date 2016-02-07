set datafile separator ","
set zeroaxis
set yrange[0:180]
set xlabel "Time (s)"
set ylabel "bpm"
# set nokey

#set xdata time
#set timestamp "%s"
#set format x "%M:%S"

# filename can be passed using gnuplot -e "filename='test.txt'" graph.gnuplot 

# set format x "%m/%d/%Y %H:%M:%S"
plot filename using 1:2 with lines title 'Polar' smooth bezier, filename using 1:3 with lines smooth bezier title 'PulseSensor'

pause -1 "Hit return to continue"
