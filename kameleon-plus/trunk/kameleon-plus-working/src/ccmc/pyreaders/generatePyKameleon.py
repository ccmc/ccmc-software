#! /usr/bin/python
import os
import sys
import shutil
sys.path.append( '.' )
# from environment import gccxml
# from pygccxml import declarations
# from pyplusplus import code_creators
from pyplusplus import module_builder
from pyplusplus.module_builder import call_policies
from pyplusplus import function_transformers as FT

if __name__ == '__main__':
	files = ['../FileReader.h', 
			'../Attribute.h', 
			'../Interpolator.h',
			]
	print 'Wrapping the following class files:'
	for file_i in files:
		print '\t', file_i
	module_name = 'pyKameleon'
	#1. module builder must point to your system's gccxml bin directory, which MUST be compiled with gcc
	# !WARNING! IF you used cmake on mac, the default compiler is Clang, not gcc!!
	# Solution: recompile gccxml using the gcc compiler (instead of clang):
	# (~/gccxml-build)$ cmake -DCMAKE_C_COMPILER=llvm-gcc-4.2 -DCMAKE_CXX_COMPILER=llvm-g++-4.2 /path/to/gccxml
	mb = module_builder.module_builder_t( files=files,
										  gccxml_path='/opt/local/bin',
	                                      # gccxml_path='/Users/apembrok/git/gccxml-build/bin',
	                                      include_paths = ['/opt/local/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7',] )

	print 'setting call policies...'

	# # call policies for GeneralFileReader class
	# generalfilereader = mb.class_("GeneralFileReader")
	# mb.print_declarations(generalfilereader)

 # 	getVariable_functions = generalfilereader.member_functions(name = 'getVariable')

 # 	for getVariable in getVariable_functions:
 # 		getVariable.call_policies = call_policies.return_value_policy(call_policies.manage_new_object)

 # 	getVariableInt = generalfilereader.member_function('getVariableInt')
 # 	getVariableInt.call_policies = call_policies.return_value_policy(call_policies.manage_new_object)
 # 	generalfilereader.include()


	attribute = mb.class_("Attribute")
	attribute.include()
	
	# setAttrFuncs = attribute.member_functions('setAttributeValue')
	# for setAttr in setAttrFuncs:
	# 	setAttr.add_transformation(FT.input(0))

	setAttrStr = attribute.mem_fun('setAttributeValue', arg_types = ['::std::string &'])
	setAttrFloat = attribute.mem_fun('setAttributeValue', arg_types = ['float &'])	
	setAttrInt = attribute.mem_fun('setAttributeValue', arg_types = ['int &'])
	
	setAttrStr.add_transformation( FT.input(0), alias = 'setAttrStr' )
	setAttrFloat.add_transformation( FT.input(0), alias = 'setAttrFloat' )
	setAttrInt.add_transformation( FT.input(0), alias = 'setAttrInt' )
	

	attribute.add_property( 'attributeName'
		, attribute.member_function('getAttributeName')
		, attribute.member_function('setAttributeName'))
	attribute.add_property( 'sValue'
		, attribute.member_function('getAttributeString')
		, setAttrStr)
	attribute.add_property( 'iValue'
		, attribute.member_function('getAttributeInt')
		, setAttrInt)
	attribute.add_property( 'fValue'
		, attribute.member_function('getAttributeFloat')
		, setAttrFloat)
	attribute.add_property( 'type'
		, attribute.member_function('getAttributeType') )	


	fileReader = mb.class_("FileReader")
 	fileReader.include()


 	print 'header full path', os.path.join( os.path.abspath('.'), '../../FileReader.h')
 	getVariable_functions = fileReader.member_functions(name = 'getVariable')#, recursive = 'true', header_file = os.path.join( os.path.abspath('.'), '../../FileReader.h'))
 	# getVariable_functions.call_policies = call_policies.custom_call_policies(call_policies.manage_new_object)

 	for getVariable in getVariable_functions:
 		getVariable.call_policies = call_policies.return_value_policy(call_policies.manage_new_object)

 	getVariableInt = fileReader.member_function('getVariableInt')
 	getVariableInt.call_policies = call_policies.return_value_policy(call_policies.manage_new_object)

 	getCurrentFilename = fileReader.member_function('getCurrentFilename')


 	fileReader.add_property( 'current_filename'
                     , fileReader.member_function( 'getCurrentFilename' )
                     , fileReader.member_function( 'setCurrentFilename' ) )

 	#replace getCurrentFilename wrapper definition
 	getCurrentFilename.exclude();
	fileReader.add_wrapper_code( 
		"""
    	virtual ::std::string const & getCurrentFilename(  ){
        	return this->current_filename;
	    }
		"""
		)

 	#exclude these? 
 	# initializeGlobalAttributes = fileReader.member_function('initializeGlobalAttributes')
 	# initializeGlobalAttributes.exclude()
 	# initializeVariableAttributes = fileReader.member_function('initializeVariableAttributes')
 	# initializeVariableAttributes.exclude()

	# mb.print_declarations(attribute)


	print 'creating module code creator with module_name ', module_name
	#2. creating module code creator
	mb.build_code_creator( module_name=module_name )   

	print 'built code_creator' 
	mb.code_creator.user_defined_directories.append( os.path.abspath( '.' ) )


	print 'writing module...'
	#4. writing module to disk
	out_file_name = os.path.join( os.path.abspath('.'), 'generated', module_name + '.cpp' )
	out_dir = os.path.join( os.path.abspath('.'), 'generated')
	if os.path.exists(out_dir):
		shutil.rmtree(out_dir)

	print 'writing module...', out_file_name
	mb.write_module( out_file_name )

	print 'done'