# Gnuplot script for plotting Pelican tutorial data.

# Set axis properties, title and labels.
set xlabel "Sample Number"
set ylabel "Signal Amplitude"
set title "Pelican Tutorial Output"
set yrange [-4:4]

# Load the data.
set datafile separator ","
plot [0:99] "pre.csv" matrix using 1:3 every 1:999:0:0 title "Pre-processed data" with linespoints 1 3
replot "post.csv" matrix using 1:3 every 1:999:0:0 title "Post-processed data" with linespoints 2 3

# Set PostScript output.
set terminal postscript enhanced colour dashed lw 1 "Helvetica" 14
set output "pelican-output.ps"
replot

# Set PNG output.
set terminal png
set output "pelican-output.png"
replot

