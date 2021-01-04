from inside this directory:

$ cp * <ns-3_dir> -r

if ns-3 is a fresh installation:

$ ./waf configure
$ ./waf build

onto the main part:
$ zsh PA_957634_main.sh

all traces are in PA_957634_helper, all plots are in PA_957634_plots.
traces and plots from my last run are in PA_957634_traces_and_plots_from_my_run

I do the plots in png, as gnuplot, for some reason, does not print the legend and the axes, when I use pdf instead of png. Afterwards all plots are put into one pdf via a tex document. If you do not have tex installed on the test machines, you can just look at the pngs, the pdf is really just a bunch of 'includegraphics'.


I talked with another student about task 4 and we shared access to our respective git repos. As of now (Sunday 11pm), parts of my implementation of task 4 is used in their project. I told them: "Wenn du auch meine TwoRay implementation abgibst, dann werden wir beide durchfallen, das waere nicht so toefte ☹" and they assured me they were still working on that task and they don't necessarily push everything right away. Probably, this will not be an issue in their final submission and they used my code only for testing and/or credited it correctly, but just in case, I wanted to be upfront about this.
