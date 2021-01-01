from inside this directory:

$ cp * <ns-3_dir> -r

if ns-3 is a fresh installation:

$ ./waf configure
$ ./waf build

onto the main part:
$ sh PA_957634_main.sh

all traces are in PA_957634_helper, all plots are in PA_957634_plots.

I do the plots in png, as gnuplot, for some reason, does not print the legend and the axes, when I use pdf instead of png. Afterwards all plots are put into one pdf via a tex document. If you do not have tex installed you can just look at the png version.
