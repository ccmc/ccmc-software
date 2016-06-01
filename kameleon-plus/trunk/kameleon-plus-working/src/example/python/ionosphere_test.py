# Warning: expect weird results if your python interpreter library doesn't match the python library used to build _CCMC.so
#  Use otool -L /path/to/_CCMC.so to confirm
import sys, argparse
@CCMC_MODULE_PATH_STR@
import _CCMC as ccmc
import os
import numpy as np
import json, base64
from collections import OrderedDict

class NumpyEncoder(json.JSONEncoder):
    def default(self, obj):
        """
        if input object is a ndarray it will be converted into a dict holding dtype, shape and the data base64 encoded
        """
        if isinstance(obj, np.ndarray):
            data_b64 = base64.b64encode(obj.data)
            return dict(__ndarray__=data_b64,
                        dtype=str(obj.dtype),
                        shape=obj.shape)
        # Let the base class default method raise the TypeError
        return json.JSONEncoder(self, obj)

def getAttributeValue(attribute):
	if attribute.getAttributeType() == ccmc.Attribute.STRING:
		return attribute.getAttributeString()
	elif attribute.getAttributeType() == ccmc.Attribute.FLOAT:
		return attribute.getAttributeFloat()
	elif attribute.getAttributeType() == ccmc.Attribute.INT:
		return attribute.getAttributeInt()

def load_dict(cdfreader, var_dict, var_names, shape = None, order = 'C'):
    for v_name in var_names:
        cdf_variable = cdfreader.getVariable(v_name)
        var_dict[v_name] = np.zeros(cdf_variable.size(), order = order)
        for i, v in enumerate(cdf_variable.iterator()):
            var_dict[v_name][i] = v
        if shape is not None:
            var_dict[v_name].resize(shape)
	
def print_metadata(cdfreader, which='global'):
	if which == 'variable':
		for i in range(cdfreader.getNumberOfVariables()):
		    varname = cdfreader.getVariableName(i)
		    print "'"+varname+"'"+",",
		    for j in range(cdfreader.getNumberOfVariableAttributes()):
		        attr_name = cdfreader.getVariableAttributeName(j+ cdfreader.getNumberOfGlobalAttributes())
		        attr = cdfreader.getVariableAttribute(varname,attr_name)
		        print '\t',attr_name, getAttributeValue(attr)

	if which == 'global':
		for i in range(cdfreader.getNumberOfGlobalAttributes()):
		    attr = cdfreader.getGlobalAttribute(cdfreader.getGlobalAttributeName(i))
		    print attr.getAttributeName(), attr.toString()

def main(argv):
	parser = argparse.ArgumentParser(description="Interpolates variables onto grid.")
	parser.add_argument("-v", "--verbose", action="count", default=0, help = 'verbosity of output')
	parser.add_argument("-input_file", metavar = 'full/path/to/input_file.cdf', type=str, help="kameleon-compatible file")
	parser.add_argument("-input_dir", metavar = 'full/path/to/input_cdf_files/', type=str, help="path to kameleon-compatible cdf files")
	parser.add_argument("-ginfo","--global-info", action='store_true', help = 'print global attributes')
	parser.add_argument("-vinfo","--variable-info", action='store_true', help = 'print attributes for all variables')
	parser.add_argument("-lon_stride", "--longitude-stride", default=1, type=int, help = 'stride of longitude - e.g. 2 is every other point')
	parser.add_argument("-lat_stride", "--latitude-stride", default=1, type=int, help = 'stride of latitude - e.g. 2 is every other point')
	parser.add_argument("-db", "--debug", action='store_true', help = 'print debug info')
	parser.add_argument("-vis", "--visualize", action = 'store_true', help = 'visualize with plotly')
	parser.add_argument("-vvar", "--vis-variable", type=str, metavar = 'var1', help = "variable to visualize. Default: first variable in variables")
	parser.add_argument("-o", "--output_file", type = str, metavar = 'path/to/output_file', help = 'output json file name and location. if not specified, will use input_file as prefix')
	parser.add_argument("-vars", "--variables", 
						type=str, nargs='+',
						metavar = ('var1','var2',), 
						# default = ('ep', 'eflux', 'eave', 'jr'),
						help='list of variables to be interpolated, e.g. -vars ep eflux eave jr')
	parser.add_argument("-wrap", "--wrap-longitude", action='store_true', help = 'repeat first longitude slice at end')
	parser.add_argument("-auto_open", action='store_true', help = 'automatically open visualized output in browser')
	parser.add_argument("-json", action='store_true', help = 'store requested variables in json file')
	parser.add_argument("-output_dir", metavar = 'full/path/to/output_dir/', type=str, help='path for json and html files to be placed. if not specified, will use --output_file as prefix')
	parser.add_argument("-store_positions", action='store_true', help = 'store theta,phi,x,y,z in json file')

	args = parser.parse_args()


	if args.input_file:
		analyse_file(args.input_file, args)
	elif args.input_dir:
		for i in os.listdir(args.input_dir):
			if i.endswith(".cdf"): 
			    input_file = i
			    analyse_file(args.input_dir + input_file, args)
	else:
		raise IOError('Need to specify -input_file or -input_dir. use -h for help')

def analyse_file(input_file, args):

	cdfreader = ccmc.CDFFileReader()

	if args.verbose: 
		print 'opening', input_file

	cdfreader.open(input_file)

	if args.global_info: 
		print_metadata(cdfreader, 'global')
	if args.variable_info:
		print_metadata(cdfreader, 'variable')
		

	variables = OrderedDict()
	
	# variable_names = ['sigmaH', 'sigmaP', 'eflux', 
	#                   'eave', 'jr', 'ep', 
	#                   'ex', 'ey', 'ez', 
	#                   'jx', 'jy', 'jz', 
	#                   'ux', 'uy', 'uz', 
	#                   'jouleheating', 
	#                   'ionnumflux']

	if args.variables:
		if args.verbose:
			print 'requested variables:', args.variables
		variable_names = args.variables
	else:
		if args.verbose: 
			print 'no variables requested.. exiting.'

		cdfreader.close()
		exit()

	if args.verbose:
		print 'loading spherical grid information..'

	sph_names = 'theta', 'phi'
	load_dict(cdfreader, variables, sph_names)


	ntheta, nphi = len(variables['theta']), len(variables['phi'])

	if args.verbose:
		print '\tinitial resolution (ntheta,nphi):', ntheta, nphi

	load_dict(cdfreader, variables, variable_names, (nphi, ntheta))
	if args.verbose: 
		print 'stored variable resolution:', variables[variable_names[0]].shape

	if args.verbose:
		print 'loading cartesian coordinates'

	cart_names = 'x', 'y', 'z'
	load_dict(cdfreader, variables, cart_names, (nphi,ntheta))


	# variables[varname][:,0] = variables[varname][:,0].mean()
	# variables[varname][:,-1] = variables[varname][:,-1].mean()
	# variables[varname][0,:] = variables[varname][-1,:]

	if args.verbose:
		print 'longitude_stride:', args.longitude_stride
		print 'latitude_stride:', args.latitude_stride

	lon_stride = slice(None,None,args.longitude_stride)
	lat_stride = slice(None,None,args.latitude_stride)

	var_subset = OrderedDict()
	for k in variables:
		if k == 'phi':
			var_subset[k] = variables[k][lon_stride]
			if args.wrap_longitude: 
				var_subset[k] = np.hstack((var_subset[k],var_subset[k][0]))
		elif k == 'theta':
			var_subset[k] = variables[k][lat_stride]
		else:
			var_subset[k] = variables[k][lon_stride,lat_stride]
			if args.wrap_longitude:
				var_subset[k] = np.vstack((var_subset[k],var_subset[k][0,:]))
	
	n0, n1 = var_subset['x'].shape

	for k in var_subset: 
		print k,var_subset[k].shape, var_subset[k].min(), var_subset[k].max()
		var_subset[k] = var_subset[k].tolist() #for json without np encoding


	if args.output_file:
		if args.output_dir: 
			output_file=args.output_dir + args.output_file
		else:
			output_file=args.output_file
	elif args.output_dir: #store in output_dir but use input_file as prefix
		output_file=args.output_dir+input_file.split('.cdf')[0].split('/')[-1]
	else: #store in same directory as input_file.cdf
		output_file=input_file.split('.cdf')[0]

	if args.verbose:
		print 'will save to prefix', output_file 

	if args.json:
		global_meta_keys = 'output_time_interval', 'standard_grid_target', 'timestep_time', 'grid_1_type'

		metadata = OrderedDict((k, getAttributeValue(cdfreader.getGlobalAttribute(k))) for k in global_meta_keys)
		metadata['central_body'] = 'earth'
		metadata['radius'] = 1.01726
		metadata['output_time_interval'] = int(metadata['output_time_interval'])
		metadata['wrapped'] = args.wrap_longitude

		meta_keys = 'units', 'actual_min', 'actual_max'
		for var in var_subset.keys():
			metadata[var] = OrderedDict((k, getAttributeValue(cdfreader.getVariableAttribute(var, k))) for k in meta_keys)

		var_json = OrderedDict()
		for k in var_subset:
			if args.store_positions:
				var_json[k] = var_subset[k]
			else:
				if k not in ('x','y','z', 'phi', 'theta'):
					var_json[k] = var_subset[k]

		meta_variables = OrderedDict([	('metadata',metadata),
										('variables',var_json)
										])
		with open(output_file + str(n0) + 'x' + str(n1) + '.json', 'w') as f:
			json.dump(meta_variables,
				f, 
				cls = json.JSONEncoder, #NumpyEncoder, 
				indent=4, 
				separators=(',',': ')
				)
		
		f.close()

	
	if args.visualize:
		try:
			import plotly
		except ImportError:
			print "cannot visualize without plotly. Please run \"pip install plotly\""
			cdfreader.close()
			exit()

		import plotly.graph_objs as go
		import plotly.tools as tools

		if args.vis_variable:
			varname = args.vis_variable
		else:
			varname = variable_names[0]

		units = str(getAttributeValue(cdfreader.getVariableAttribute(varname, 'units')))
		x, y, z = var_subset['phi'],var_subset['theta'], np.asarray(var_subset[varname])

		if args.wrap_longitude: # remove wrapped line
			np.delete(z,-1,0)
			x.pop()

		z = z.T.tolist() #transposes list of lists
		contours = go.Contour(
	        x=x,
	        y=y,
	        z=z,
	        contours=dict(
	            			coloring='heatmap' #or 'contour' for flat
	        			),
	        colorbar = go.ColorBar(title=varname + ' [' + units + ']')
		    )

		surface = go.Surface(x=var_subset['x'], 
		                     y=var_subset['y'], 
		                     z = var_subset['z'], 
		                     surfacecolor=var_subset[varname], 
		                     showscale=False,
		                     )

		fig = tools.make_subplots(rows=1, cols=2, specs=[[{'is_3d': False}, {'is_3d': True}]])

		# fig['layout'].update(height=600, width=600, title=args.input_file.split('/')[-1] + '<br>' + varname)

		fig.append_trace(contours, 1, 1)
		fig.append_trace(surface, 1, 2)

		plotly.offline.plot(fig, filename=output_file + str(n0) + 'x' + str(n1) +'.html', auto_open=args.auto_open)


	cdfreader.close()



if __name__ == '__main__':
    main(sys.argv[1:])
