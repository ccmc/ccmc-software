#!/usr/bin/perl -w

=pod

	NAME:

		deploy_full_kameleon.pl

	AUTHOR:
		Marlo M. H. Maddox

	PURPOSE:
		automatically unpack, build, and deploy
		kameleon software suite and dependencies
=cut

use Term::ANSIColor qw(:constants);

my $unpack_in_place_command = "tar -xzf";
my $untar_in_place_command = "tar -xf";
my $installation_dir = "/usr/local/";

## my $make_options = "-s";

my $make_options = "";


#### SET Variables for expected tgz files, make macros, and directories

my $kameleon_dependencies_dir = "kameleon_dependencies";

## Argtable

my $unpacked_argtable2_tgz_dir = $argtable2_tgz_file = "argtable2-12.tar.gz";
chomp($unpacked_argtable2_tgz_dir);  
$unpacked_argtable2_tgz_dir =~ s/\.tar.gz//i;

## CDF

my $unpacked_cdf_tgz_dir = $cdf_tgz_file = "cdf33_0-dist-all.tar.gz";
chomp($unpacked_cdf_tgz_dir);  
$unpacked_cdf_tgz_dir =~ s/\-all\.tar.gz//i;

my $cdf_make_options = "OS=linux ENV=gnu FORTRAN=yes FC_linux=gfortran CC_linux_gnu=gcc all";

## FITS

my $unpacked_fits_tgz_dir = $fits_tgz_file = "cfitsio3250.tar.gz";
chomp($unpacked_fits_tgz_dir);  
$unpacked_fits_tgz_dir =~ s/3250\.tar.gz//i;

my $fits_configure_options = "--prefix=$installation_dir";


## JPEG

my $unpacked_jpeg_tgz_dir = $jpeg_tgz_file = "jpegsrc.v6b.tar";
chomp($unpacked_jpeg_tgz_dir);  
$unpacked_jpeg_tgz_dir =~ s/src\.v6b\.tar/\-6b/i;


## SZIP

my $unpacked_szip_tgz_dir = $szip_tgz_file = "szip-2.1.tar.gz";
chomp($unpacked_szip_tgz_dir);  
$unpacked_szip_tgz_dir =~ s/\.tar\.gz//i;

my $szip_configure_options = "--prefix=$installation_dir";

## ZLIB

my $unpacked_zlib_tgz_dir = $zlib_tgz_file = "zlib-1.2.5.tar.gz";
chomp($unpacked_zlib_tgz_dir);  
$unpacked_zlib_tgz_dir =~ s/\.tar\.gz//i;

## HDF4

my $unpacked_hdf4_tgz_dir = $hdf4_tgz_file = "hdf-4.2.5.tar.gz";
chomp($unpacked_hdf4_tgz_dir);  
$unpacked_hdf4_tgz_dir =~ s/\.tar\.gz//i;

my $hdf4_configure_options = "--prefix=$installation_dir --disable-netcdf";
my $hdf4_env_options = "PATH=/usr/local/lib:/usr/local/bin:/usr/local/include/:/usr/local/lib64:/usr/local/sbin/:{\$PATH} LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64/ CFLAGS=\"-fPIC -DHAVE_NETCDF\" CXXFLAGS=\"-fPIC -DHAVE_NETCDF\" LIBS=\"-lm\"";


## netCDF

my $unpacked_netcdf_tgz_dir = $netcdf_tgz_file = "netcdf-4.1.1.tar.gz";
chomp($unpacked_netcdf_tgz_dir);  
$unpacked_netcdf_tgz_dir =~ s/\.tar\.gz//i;

my $netcdf_configure_options = "--prefix=$installation_dir --enable-shared --disable-netcdf4 --disable-dap";


## Kameleon Converter

my $unpacked_kameleon_converter_tgz_dir = $kameleon_converter_tgz_file = "kameleon-converter-v5.2.0.tgz";
chomp($unpacked_kameleon_converter_tgz_dir);  
$unpacked_kameleon_converter_tgz_dir =~ s/\.tgz//i;

my $kameleon_converter_make_options = "CC=gcc F77=gfortran LIB_PATH=\"-L/usr/local/lib/\" LIB_CDF=/usr/local/lib/libcdf.a CDF_INC=/usr/local/include/ LIB_ARG_TABLE_2=/usr/local/lib/libargtable2.a ARG_TABLE_2_INC=/usr/local/include/ LIB_NETCDF=/usr/local/lib/libnetcdf.a NETCDF_INC=/usr/local/include/ LIB_FITS=/usr/local/lib/libcfitsio.a FITS_INC=/usr/local/include/ LIB_DF=/usr//local/lib/libdf.a LIB_MFHDF=/usr/local/lib/libmfhdf.a HDF_INC=/usr/local/include/ LIB_SZ=/usr/local/lib/libsz.a LIB_Z=/usr/local/lib/libz.a";


## Kameleon Interpolator

my $unpacked_kameleon_interpolator_tgz_dir = $kameleon_interpolator_tgz_file = "kameleon-interpolator-v5.0.0.tgz";
chomp($unpacked_kameleon_interpolator_tgz_dir);  
$unpacked_kameleon_interpolator_tgz_dir =~ s/\.tgz//i;

my $kameleon_interpolator_make_options = "CC=gcc F77=gfortran LIB_CDF=/usr/local/lib/libcdf.a CDF_INC=/usr/local/include/";

#### END SET Variables for expected tgz files, make macros, and directories


my $DEBUG_FLAG = 0;
my $number_of_seconds_to_sleep = 3;

my $system_command_status = "-1";
my $unpacked_kameleon_directory = $kameleon_tgz_file = $ARGV[0];
chomp($unpacked_kameleon_directory);
$unpacked_kameleon_directory =~ s/\.tgz//i;


my $current_system_command = "";

my $current_working_directory = `pwd`;
chomp($current_working_directory);

my $current_user = `whoami`;
chomp($current_user);

my $make_install = "";
my $cdf_make_install = "";



	if($current_user =~ /^root/)
	{
		$make_install = "make $make_options install";
		$cdf_make_install = "make $make_options INSTALLDIR=$installation_dir/ install";

	}



	if($current_user !~ /^root/)
	{

	print "\n\n--------------- EXECUTING AS USER: $current_user --------------- \n\n...please note that installation into /usr/local/ requires this program to be executed as root!!!!\n\nCommening with build in $number_of_seconds_to_sleep seconds...\n\n";


	sleep($number_of_seconds_to_sleep);

	}


#################################### INITIAL SETUP COMPLETE BEGIN BUILD STEPS ################################


### Unpack The Main Distribution File

$current_system_command = "$unpack_in_place_command $current_working_directory/$kameleon_tgz_file";

run_command($current_system_command);

### CD into recently unpacked file

change_directory_into("$current_working_directory/$unpacked_kameleon_directory");

### CD into kameleon_dependencies/

change_directory_into("$current_working_directory/$kameleon_dependencies_dir");

#################################################### ARGTABLE BUILD

### Unapck The Argtable 2 Library

$current_system_command = "$unpack_in_place_command $current_working_directory/$argtable2_tgz_file";

run_command($current_system_command);

### CD into recently unpacked file

change_directory_into("$current_working_directory/$unpacked_argtable2_tgz_dir");

### Build the Argatble 2 distribution

$current_system_command = "CC=gcc;F77=gfortran;$current_working_directory/configure;make $make_options;$make_install;";

run_command($current_system_command);

### CD up one level back into kameleon_dependencies/

change_directory_into("../");

#################################################### CDF BUILD

### Unapck The CDF Library

$current_system_command = "$unpack_in_place_command $current_working_directory/$cdf_tgz_file";

run_command($current_system_command);

### CD into recently unpacked file

change_directory_into("$current_working_directory/$unpacked_cdf_tgz_dir");

### Build the CDF distribution

$current_system_command = "CC=gcc;F77=gfortran;make $make_options $cdf_make_options;$cdf_make_install;";

run_command($current_system_command);


### CD up one level back into kameleon_dependencies/

change_directory_into("../");

#################################################### FITS BUILD

### Unapck The FITS Library

$current_system_command = "$unpack_in_place_command $current_working_directory/$fits_tgz_file";

run_command($current_system_command);

### CD into recently unpacked file

change_directory_into("$current_working_directory/$unpacked_fits_tgz_dir");

### Build the FITS distribution

$current_system_command = "CC=gcc;F77=gfortran;$current_working_directory/configure $fits_configure_options;make $make_options;$make_install;";

run_command($current_system_command);





### CD up one level back into kameleon_dependencies/

change_directory_into("../");

#################################################### JPEG BUILD

### Unapck The JPEG Library

$current_system_command = "$untar_in_place_command $current_working_directory/$jpeg_tgz_file";

run_command($current_system_command);

### CD into recently unpacked file

change_directory_into("$current_working_directory/$unpacked_jpeg_tgz_dir");

### Build the JPEG distribution

$current_system_command = "CC=gcc;F77=gfortran;$current_working_directory/configure;make $make_options;mkdir -p /usr/local/man/man1;$make_install;";

run_command($current_system_command);



### CD up one level back into kameleon_dependencies/

change_directory_into("../");

#################################################### SZIP BUILD

### Unapck The SZIP Library

$current_system_command = "$untar_in_place_command $current_working_directory/$szip_tgz_file";

run_command($current_system_command);

### CD into recently unpacked file

change_directory_into("$current_working_directory/$unpacked_szip_tgz_dir");

### Build the SZIP distribution

$current_system_command = "CC=gcc;F77=gfortran;$current_working_directory/configure $szip_configure_options;make $make_options;$make_install;";

run_command($current_system_command);


### CD up one level back into kameleon_dependencies/

change_directory_into("../");

#################################################### ZLIB BUILD

### Unapck The ZLIB Library

$current_system_command = "$untar_in_place_command $current_working_directory/$zlib_tgz_file";

run_command($current_system_command);

### CD into recently unpacked file

change_directory_into("$current_working_directory/$unpacked_zlib_tgz_dir");

### Build the ZLIB distribution

$current_system_command = "CC=gcc;F77=gfortran;$current_working_directory/configure;make $make_options test;$make_install;";

run_command($current_system_command);



### CD up one level back into kameleon_dependencies/

change_directory_into("../");

#################################################### HDF4 BUILD

### Unapck The HDF4 Library

$current_system_command = "$unpack_in_place_command $current_working_directory/$hdf4_tgz_file";

run_command($current_system_command);

### CD into recently unpacked file

change_directory_into("$current_working_directory/$unpacked_hdf4_tgz_dir");

### Build the HDF4 distribution

$current_system_command = "CC=gcc;F77=gfortran;$hdf4_env_options;$current_working_directory/configure $hdf4_configure_options;make $make_options;make $make_options check;$make_install;";

run_command($current_system_command);


### CD up one level back into kameleon_dependencies/

change_directory_into("../");

#################################################### netCDF BUILD

### Unapck The netCDF Library

$current_system_command = "$unpack_in_place_command $current_working_directory/$netcdf_tgz_file";

run_command($current_system_command);

### CD into recently unpacked file

change_directory_into("$current_working_directory/$unpacked_netcdf_tgz_dir");

### Build the netCDF distribution

$current_system_command = "CC=gcc;F77=gfortran;$current_working_directory/configure $netcdf_configure_options;make $make_options;make check;$make_install;";

run_command($current_system_command);



################################################### Kameleon Dependencies Done ##################################################


### CD up two levels back into kameleon/

change_directory_into("../../");

#################################################### Kameleon Converter BUILD

### Unapck The Kameleon Converter Library

$current_system_command = "$unpack_in_place_command $current_working_directory/$kameleon_converter_tgz_file";

run_command($current_system_command);

### CD into recently unpacked file

change_directory_into("$current_working_directory/$unpacked_kameleon_converter_tgz_dir");

### Build the Kameleon Converter distribution

$current_system_command = "make $kameleon_converter_make_options;make clean";

run_command($current_system_command);

### Deploy Kameleon executable....


$current_system_command = "ln -fs $current_working_directory/kameleon $installation_dir/bin/kameleon";

run_command($current_system_command);

### CD back one level into kameleon/

change_directory_into("../");


#################################################### Kameleon Interpolator BUILD

### Unapck The Kameleon Interpolator Library

$current_system_command = "$unpack_in_place_command $current_working_directory/$kameleon_interpolator_tgz_file";

run_command($current_system_command);

### CD into recently unpacked file

change_directory_into("$current_working_directory/$unpacked_kameleon_interpolator_tgz_dir");

### Build the Kameleon Interpolator distribution

$current_system_command = "make $kameleon_interpolator_make_options;make clean";

run_command($current_system_command);

### Deploy Kameleon executable....





sub run_command
{

	my $current_system_command = $_[0];

	my $system_command_status = system("$current_system_command ");

	print "DEBUG:\t$current_system_command  RETURNED--->$system_command_status<---\n" if( $DEBUG_FLAG );

	if( $system_command_status == 0 )
	{
		print "[";
		print GREEN, "OK", RESET;
		print "] $current_system_command \n";
	}
	else
	{	print "[";
		print RED, "FAILED", RESET;
		print "] $current_system_command \n...EXITING $0\n";
		exit(-1);
	}


}

sub change_directory_into
{

	my $target_directory = $_[0];

	my $current_system_command = "cd $target_directory";

	my $system_command_status = 1;

	$system_command_status = 0 if( chdir($target_directory) );

	$current_working_directory = `pwd`;
	chomp($current_working_directory);

	print "DEBUG:\t$current_system_command RETURNED--->$system_command_status<---\n" if( $DEBUG_FLAG );

	if( $system_command_status == 0 )
	{
		print "[";
		print GREEN, "OK", RESET;
		print "] $current_system_command \n";
	}
	else
	{	print "[";
		print RED, "FAILED", RESET;
		print "] $current_system_command \n...EXITING $0\n";
		exit(-1);
	}



}




