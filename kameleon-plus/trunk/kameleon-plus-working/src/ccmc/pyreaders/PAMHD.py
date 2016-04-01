#! /usr/bin/env python3
'''
Function for reading output of PAMHD MHD test program.

Copyright 2015 Ilja Honkonen
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

* Neither the name of copyright holders nor the names of their contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


If called as the main program creates an empty dictionary mhd_data
and fills it by calling load() on each file given as an argument to
this program.

Example printing the id of every cell in one file:
python -i mhd2numpy.py mhd_0.000e+00_s.dc
for key in mhd_data:
	print(key)

Example printing the magnetic field in cell 3 in the first given file:
python -i mhd2numpy.py mhd_0.000e+00_s.dc
print(mhd_data[3][0][3])

Example plotting y component of magnetic field from all cells in
first given file as a function of cells' x coordinate (looks best
with 1-dimensional simulation):
python -i mhd2numpy.py mhd_0.000e+00_s.dc
x = []
By = []
[x.append(mhd_data[cell_id][0][0][0]) for cell_id in mhd_data]
[By.append(mhd_data[cell_id][0][5][1]) for cell_id in mhd_data]
import matplotlib.pyplot as plot
plot.plot(x, By)
plot.show()
'''

import os

try:
	import numpy
except:
	exit("Couldn't import numpy")

from collections import namedtuple

'''
Returns data of PAMHD MHD test program output.

Does nothing and returns None if mhd_data is None.

Returns the following in a numpy array:
simulation time,
adiabatic index,
proton_mass,
vacuum permeability.

mhd_data will have cell ids as keys and each value is a list of
tuples of following items:
coordinate of cell's center,
cell's length,
mass density,
momentum density,
total energy density,
magnetic field,
electric current density,
cell type,
mpi rank,
electric resistivity.

Every call appends only one tuple to each key, i.e. if called
on an empty dictionary every list will have only one tuple.
'''

def load(file_name, mhd_data):
	SimParams = namedtuple('SimParams',['time', 'adiabatic_index', 'proton_mass', 'vacuum_permeability'])

	MHD_Components = namedtuple('MHD_Components', [ 'c0','c1','c2', 
													# 'cell_length', 
													'mass_density', 
													'px','py','pz', 
													'total_energy_density',
													'bx','by','bz',
													'jx','jy','jz',
													'cell_type',
													'mpi_rank',
													'electric_resistivity',
													])

	if mhd_data == None:
		return None

	if not os.path.isfile(file_name):
		raise Exception('Given file name (' + file_name + ') is not a file.')

	# infile = open(file_name, 'rb')
	with open(file_name, 'rb') as infile: #will close safely and automatically

		# read simulation header, given by source/mhd/save.hpp
		file_version = numpy.fromfile(infile, dtype = 'uint64', count = 1)
		if file_version > 1:
			print('Warning file version is newer than expected...')
		sim_params = numpy.fromfile(infile, dtype = '4double', count = 1)

		# from this point onward format is given by dccrg's save_grid_data()
		# check file endiannes
		endianness = numpy.fromfile(infile, dtype = 'uint64', count = 1)[0]
		if endianness != numpy.uint64(0x1234567890abcdef):
			# should swap endiannes here
			raise Exception(
				'Wrong endiannes in given file, expected ' + str(hex(0x1234567890abcdef)) \
				+ ' but got ' + str(hex(endianness))
			)

		# number of refinement level 0 cells in each dimension
		ref_lvl_0_cells = numpy.fromfile(infile, dtype = '3uint64', count = 1)[0]
		#print(ref_lvl_0_cells)

		# maximum refinement level of grid cells
		max_ref_lvl = numpy.fromfile(infile, dtype = 'intc', count = 1)[0]
		if max_ref_lvl > numpy.uint32(0):
			raise Exception('Refinement level > 0 not supported')

		# length of every cells' neighborhood in cells of identical size
		neighborhood_length = numpy.fromfile(infile, dtype = 'uintc', count = 1)[0]
		#print(neighborhood_length)

		# whether grid is periodic each dimension (0 == no, 1 == yes)
		periodicity = numpy.fromfile(infile, dtype = '3uint8', count = 1)[0]
		#print(periodicity)

		geometry_id = numpy.fromfile(infile, dtype = 'intc', count = 1)[0]
		if geometry_id != numpy.int32(1):
			raise Exception('Unsupported geometry')

		# starting coordinate of grid
		grid_start = numpy.fromfile(infile, dtype = '3double', count = 1)[0]
		#print(grid_start)

		# length of cells of refinement level 0
		lvl_0_cell_length = numpy.fromfile(infile, dtype = '3double', count = 1)[0]
		#print(lvl_0_cell_length)

		# total number of cells in grid
		total_cells = numpy.fromfile(infile, dtype = 'uint64', count = 1)[0]
		#print(total_cells)

		# id of each cell and offset in bytes to data of each cell
		cell_ids_data_offsets = numpy.fromfile(infile, dtype = '2uint64', count = total_cells)
		#print(cell_ids_data_offsets)

		# until this point format decided by dccrg
		# from this point onward format decided by save() call of tests/mhd/test.cpp

		# read particle data
		for item in cell_ids_data_offsets:
			cell_id = item[0]

			# calculate cell geometry, defined by get_center() function in 
			# dccrg_cartesian_geometry.hpp file of dccrg
			cell_id -= 1
			cell_index = (
				int(cell_id % ref_lvl_0_cells[0]),
				int(cell_id / ref_lvl_0_cells[0] % ref_lvl_0_cells[1]),
				int(cell_id / (ref_lvl_0_cells[0] * ref_lvl_0_cells[1]))
			)

			cell_center = (
				grid_start[0] + lvl_0_cell_length[0] * (0.5 + cell_index[0]),
				grid_start[1] + lvl_0_cell_length[1] * (0.5 + cell_index[1]),
				grid_start[2] + lvl_0_cell_length[2] * (0.5 + cell_index[2])
			)
			cell_id += 1

			infile.seek(item[1], 0)
			temp = numpy.fromfile(
				infile,
				dtype = 'double, 3double, double, 3double, 3double, intc, intc, double',
				count = 1
			)[0]

			data = MHD_Components(
				c0 = cell_center[0],
				c1 = cell_center[1],
				c2 = cell_center[2],
				# cell_length = lvl_0_cell_length,
				mass_density = temp[0],
				px = temp[1][0],
				py = temp[1][1],
				pz = temp[1][2],
				total_energy_density = temp[2],
				bx = temp[3][0],
				by = temp[3][1],
				bz = temp[3][2],
				jx = temp[4][0],
				jy = temp[4][1],
				jz = temp[4][2],
				cell_type = temp[5],
				mpi_rank = temp[6],
				electric_resistivity = temp[7]
			)

			if cell_id in mhd_data:
				mhd_data[cell_id].append(data)
			else:
				mhd_data[cell_id] = [data]

	return SimParams(*sim_params[0])


if __name__ == '__main__':
	import sys

	mhd_data = dict()
	for arg in sys.argv[1:]:
		load(arg, mhd_data)