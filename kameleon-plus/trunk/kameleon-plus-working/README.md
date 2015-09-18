# Kameleon-plus 6.1.0 #


## Requirements ## 

* cmake 2.8
* hdf5 (LFM support - optional)
* [cdf3.5](http://cdaweb.gsfc.nasa.gov/pub/software/cdf/dist/cdf35_0_2/)
* boost 1.54.0
* swig

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

##Test ## 
###Testing c++ executable###

	cd path/to/ccmc-software/kameleon-plus/trunk/kameleon-plus-working/bin/examples/c++
	./kameleon_prog /path/to/ccmc-converted/file.cdf bz -30 0 0

###Testing python scripts###

	cd path/to/ccmc-software/kameleon-plus/trunk/kameleon-plus-working/bin/examples/python
	python grid.py /path/to/ccmc-converted/file.cdf -ginfo

See grid.py [documentation](https://github.com/ccmc/ccmc-software/blob/python_interpolators/kameleon-plus/trunk/kameleon-plus-working/src/example/python/README.md) for options

## Switch to an experimental branch ##
	git checkout <experimental branch name>