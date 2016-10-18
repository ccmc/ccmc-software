# Kameleon-plus 6.1.0 #

## NEWS ##

Sept, 2016:
        - Added generic ASDF reader
        - Fixed batsrus "vis" units for high-level kameleon object interface
        - Updated IGRF to v12 for coorinate transformations (see https://github.com/edsantiago/cxform)

We have discovered a unit-conversion bug in the kameleon software suite, brought to our attention by Seth Claudpierre, affecting kameleon-plus BATS-R-US users. Specifically, when using the high-level kameleon object interpolator, units of pressure (nPa) are lower than they should be by a factor of 1000, while the units of plasma beta are off by 1e9. Fortunately, this does not affect the runs-on-request visualization, which uses a different set of tools for interpolating BATSRUS.

## Requirements ## 

### Conda environment ###

We have prepared a conda environment for kameleon developers on OSX, which may be found [here](https://github.com/ccmc/conda-recipes/tree/master/environments/kameleon_dev). Otherwise, you will need these requirements.

### build ###
	* python 
	* hdf5 1.8.14 [osx]
	* hdf5 1.8.15.1 2 [linux]
	* cmake >=2.8
	* [cdf 3.6](http://cdaweb.gsfc.nasa.gov/pub/software/cdf/dist/cdf36_1/linux/cdf36_1-dist-all.tar.gz)
	* boost 1.59.0
	* swig

### run ###
	* boost 1.59.0
	* python 
	* [cdf 3.6](http://cdaweb.gsfc.nasa.gov/pub/software/cdf/dist/cdf36_1/linux/cdf36_1-dist-all.tar.gz)
	* hdf5 1.8.14 [osx]
	* hdf5 1.8.15.1 2 [linux]
	* numpy
	* matplotlib

## Download##
```console
git clone https://github.com/ccmc/ccmc-software.git
```
## Update to latest stable build ##
	cd /path/to/ccmc-software
	git pull 

## Build ##
Start a fresh build directory to populate with makefiles

	mkdir ~/Kameleon-plus-build
	cd ~/Kameleon-plus-build

Run from the build directory and point it to the kameleon-plus source. Using -j8 speeds up compilation time:

	cmake /path/to/ccmc-software/kameleon-plus/trunk/kameleon-plus-working
	make -j8 

The make command compiles the source places the following example c++ excuteables in ```/path/to/ccmc-software/kameleon-plus/trunk/kameleon-plus-working/bin/examples/c++```:

	adapt3d_test  coordinate_transformation_test  IMFTest
	kameleon_prog  magnetogram_test	open_ggcm_test	time_interp
	CDFReader  enlil_test  integrator_prog  lfm_test
	mas_test  swmf_iono_test	tracer_prog

It also places the following python programs in ```path/to/ccmc-software/kameleon-plus/trunk/kameleon-plus-working/bin/examples/python```

## Installing ##
Install the CCMC python module (finicky):
	make install

## Test ## 
### Testing c++ executable###

	cd path/to/ccmc-software/kameleon-plus/trunk/kameleon-plus-working/bin/examples/c++
	./kameleon_prog /path/to/ccmc-converted/file.cdf bz -30 0 0

### Testing python scripts###

	cd path/to/ccmc-software/kameleon-plus/trunk/kameleon-plus-working/bin/examples/python
	python grid.py /path/to/ccmc-converted/file.cdf -ginfo

See grid.py [documentation](https://github.com/ccmc/ccmc-software/blob/python_interpolators/kameleon-plus/trunk/kameleon-plus-working/src/example/python/README.md) for options

## Switch to an experimental branch ##
	git checkout <experimental branch name>
