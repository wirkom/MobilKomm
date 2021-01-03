from inside this directory:

$ cp * <ns-3_dir> -r

if ns-3 is a fresh installation:

$ ./waf configure
$ ./waf build

onto the main part:
$ sh PA_957634_main.sh

all traces are in PA_957634_helper, all plots are in PA_957634_plots.

I do the plots in png, as gnuplot, for some reason, does not print the legend and the axes, when I use pdf instead of png. Afterwards all plots are put into one pdf via a tex document. If you do not have tex installed on the test machines, you can just look at the pngs, the pdf is really just a bunch of 'includegraphics'.


I talked with another student about task 4 and we shared access to our respective gitlab repos. As of now (Sunday 9pm), my code is used in their project (with slight changes, like writing i*i instead of std::pow(i,2) ). I told them: "Wenn du auch meine TwoRay implementation abgibst, dann werden wir beide durchfallen, das waere nicht so toefte ☹" and they told me they were going to change stuff. I hope this will not be an issue in their final submission, but just in case, I write this.
