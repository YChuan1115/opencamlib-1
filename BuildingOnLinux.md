# Introduction #

Instructions on how to compile OCL on Linux.

# Details #

## Prerequisites ##

The minimal set of software needed to build the OpenCamLib Python module on Linux (as of [r128](https://code.google.com/p/opencamlib/source/detail?r=128)) is:
  * cmake
  * libboost-python-dev
  * python-dev
  * OpenMP

If you want to also build the documentation, install these additional packages:
  * doxygen
  * texlive-latex-base

## Checking out OpenCamLib ##

First check out a copy of the OCL Subversion repository, as shown in the Google Code [checkout instructions](http://code.google.com/p/opencamlib/source/checkout).


## Building ##

OCL uses [cmake](http://cmake.org) to help with cross-platform building.  The first thing you need to do is use cmake to create your Makefiles:
```
cd src
cmake .
```
Running cmake produces the Makefiles you need to compile the software.  You only need to run cmake the first time you check out the repo, it will be automatically rerun in the future if it's needed.

Now you're ready to build everything.  Three important make targets are defined:  ocl, doc, and all.  The "ocl" target builds just the ocl Python module.  The "doc" target builds all the documentation (assuming you installed Doxygen and texlive-latex-base).  The "all" target builds both ocl and doc.

```
make ocl
```
Or just this, to build the docs too:
```
make
```

## Installing ##
If you want to install ocl.so and camvtk.py (the VTK visualization helper library) so that they are available for any python script to use, then run:
```
sudo make install
```

That's it!  Have fun!