# Optimization and analysis tools

Note: The Makefiles and PBS scripts in the `water` repository have been
updated to illustrate how to use some of the following tools.

## Intel's compilers

The Intel compiler can generate reports that tell how successful it
was at vectorizing your code.  For example, compiling with the flags

    -qopt-report=5 -qopt-report-phase=vec

will give a detailed report (`*.optrpt`) that says what is and is not
successfully vectorized.  Note that the report may talk about several
different types of dependencies; there is a reasonable
[Wikipedia article][wiki-hazard] covering the terminology that is used.

Note that there are several references on Intel's web pages that
describe how to [annotate functions for vectorization][simd-annotations]
and how to [vectorize step-by-step][vec-tutorial], as well as a
[more thorough guide to vectorization][vec-guide].  You will probably
see a significant speedup for this assignment by following the
vectorization guidelines.

[wiki-hazard]: https://en.wikipedia.org/wiki/Dependence_analysis
[vec-tutorial]: https://software.intel.com/en-us/articles/step-by-step-optimizing-with-intel-c-compiler
[vec-guide]: https://software.intel.com/sites/default/files/8c/a9/CompilerAutovectorizationGuide.pdf
[simd-annotations]: https://software.intel.com/en-us/node/512636

## VTune Amplifier

We can't run the VTune GUI from our head node, but we can use
the command line interface `amplxe-cl`.  This is available whenever
you have the `cs5220` module loaded.  The
[reference guide][amplxe-guide] for the command-line tool
is hard to find by a Google search,
but it is up on the Intel web site.  You can also get an
overview of the options by running

    amplxe-cl -help

Before using data with Amplifier, you will want to compile your
code with debugging symbols (`-g` flag).  From there, profiling
proceeds in two phases: data collection and reporting.

[amplxe-guide]: https://software.intel.com/en-us/node/544218

### Data collection

The collection phase will run on the compute nodes in the cluster.
To run an advanced hotspot diagnostic, for instance,
execute (from a PBS script)

    amplxe-cl -collect advanced-hotspots ./shallow

It is possible to collect a wide variety of data, including not only
hotspots but also counter information, memory bandwidth utilization,
etc.  The data will be deposited in a subdirectory of the directory
where the code was run; for example, the first time the above
collection is run, it will create a directory `r001ah`.

### Reporting

Once the data has been collected, reports can be generated on the
front-end node.  For example, to get an overview of the hotspots, try

    amplxe-cl -report hotspots -r r001ah/

To see the time spent in `compute_step`, try something like

    amplxe-cl -report hotspots -source-object function="Central2D<Shallow2D, MinMod<float>>::compute_step"

## TAU

The [Tuning and Analysis Utilities][tau] suite from University of
Oregon supports a variety of profiling methods, including
source-to-source translation, dynamic instrumentation of binaries,
and statistical sampling.  You can load TAU using the `tau` modules
on Totient.  Unfortunately, the source-to-source translation doesn't
seem to work with recent C++ dialects, and I have not yet figured
out how to link with the dynamic instrumentation libraries
provided as a part of the RedHat Devtoolset-3.

[tau]: https://www.cs.uoregon.edu/research/tau/home.php

## IACA

I was really excited to learn about the
[Intel Architecture Code Analyzer][iaca];
the [user guide][iaca-ug] makes it look quite handy,
and I've read some [interesting posts][iaca-post]
about it online.  Unfortunately, IACA is only released in binary
form, and the binaries are linked to a more recent version of
`glibc` (and `libstdc++`) than we have installed on the cluster.
Feel free to run it on your own machines, though!

## MAQAO

[MAQAO (Modular Assembly Quality Analyzer and Optimizer)][maqao]
is a relatively new tool that provides detailed analysis of
the (potential) shortcomings of your code based on a static
analysis of the generated assembly language.  We have the analysis
tools for both 64-bit x86 and Xeon Phi; try `module load maqao`.
