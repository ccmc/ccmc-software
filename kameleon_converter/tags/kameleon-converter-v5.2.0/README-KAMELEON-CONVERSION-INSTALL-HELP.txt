09.12.2006
Community Coordinated Modeling Center
NASA Goddard Space Flight Center

The ccmc KAMELEON conversion software depends on external libraries that must be installed first:

(1) libcdf.a - acquired via CDF 3.0. CDF is provided by The National Space Science Data Center at NASA Goddard Space Flight Center.
http://nssdc.gsfc.nasa.gov/cdf/cdf_home.html

(2) libargtable2.a -  acquired via argtable2 ( http://argtable.sourceforge.net/ Stewart Heitmann - sheitmann@users.sourceforge.net ) - this distribution uses argtable2.4.  Argtable version 2.3 is also compatible.

NOTE:  If you are currently using CDF 2.7 AND/OR using argtable2.3, copy the cdf.h & argtable2.h header files into the local inc directory for the ccmc-converter distribution.  The CCMC converter works with CDF2.7 or CDF3.0 and argtable2.3 or argtable2.4.

(3) libnetcdf.a - aquired via netCDF - netcdf-3.6.0-p1 is provided by http://www.unidata.ucar.edu/software/netcdf/

NOTE: Step 3 is optional as libnetcdf.a is only required for model output that is in the netCDF format.  The distribution will still build if the libnetcdf.a static library is unavailable.

------------------------------------------------------------------------------

For an in depth look into CDF, read the CDF  Users Guide ( cdf30ug.pdf )   and/or the  Fortran/ C Reference Manuals ( cdf30crm.pdf & cdf30frm.pdf ) located in the included cdf distribution.  The original README files and release notes are also provided.

For an in-depth look into netCDF go to http://www.unidata.ucar.edu/packages/netcdf/docs ...

This file, README.txt, is just a summary of how to get started quickly.

---------------------------------------------------------------------------------

Summary of how to install CDF version 3.0

The cdf30-dist-all.tar.gz file contains the CDF distribution.

1.    Copy the cdf30-dist-all.tar.gz file to the location were you would like to build the CDF installation.

    cp cdf30-dist-all.tar.gz /user_specified_path

2.    Uncompress the distribution

    gzip -d cdf30-dist-all.tar.gz

3.    "Un-Tar" the tar file

    tar -x -v -f cdf30-dist-all.tar

4.    cd into the newly created cdf30-dist directory

    cd cdf30-dist

5.    determine what make variables need to be set for your particular platform.  There are seven make variables that can be set.      Two of the make variables are required.

    OS        REQUIRED.  The operating system being used.
    ENV        REQUIRED.  The environment begin used (eg. C compiler).
    SHARED        Optional.  Should the shareable CDF library be built?
    CURSES        Optional.  Should Curses-based toolkit programs be built?
    FORTRAN        Optional.  Should the Fortran interface be tested?
    IDL               Optional.  Should CDF's IDL interface be built?
    IDLINCLUDE    May be required if CDF's IDL interface is built.

  To see all of the options enter:

    make all.help

6.    compile the distribution

  base example for linux os with gnu environment

  make OS=linux ENV=gnu all

  example2: for linux os with gnu environment with IDL & Fortran option

  make OS=linux ENV=gnu FORTRAN=yes IDL=yes IDLINCLUDE=/opt/local/rsi/idl_5.3/external all

  NOTE: all keyword at end of command line

7.    test the compilation

  make test

8.    install the distribution

  make INSTALLDIR=/your_install_dir install

  NOTE: install keyword at end of command line

  We are mainly interested in the libcdf.a static library for use with the converter & interpolator.  Additional steps are optional.

9.    ( optional ) set your environment variables for quick access to the cdf tools

  cd into the directory in which the cdf distribution was installed. ie. your_install_dir

  cd into the bin directory

  read the definitions.* file for your particular shell ie. definitions.C for C-shell and tcsh

  set the CDF_BASE variable to the correct directory in which cdf is installed

  follow the instructions to run the script ie. source <path>/bin/definitions.C or

  add the command(s) manually to your particular shell i.e. .cshrc

10.      ( optional )read a cdf file using cdfexport or cdfedit

  cdfexport <path>/cdf_file_name_no_extension

  ie. cdfexport <path>/3d__ful_1_20000101_01000_0175.out

11.    ( optional ) Another useful tool is skeletontable.  It creates a text file that describes the structure of the     cdf file

  skeletontable -skeleton 3d__ful_1_20000101_01000_0175.out <path>/3d__ful_1_20000101_01000_0175.out.cdf

  this command will create a file 3d__ful_1_20000101_01000_0175.out.skt from the 3d__ful_1_20000101_01000_0175.out.cdf
  you can then open the .skt file with any text editor to browse the contents.  The -skeleton argument just specifies what to name the output ( .skt ) file.  Note that the suffix .skt will be appended to this argument on output.

---------------------------------------------------------------------------------

Summary of how to install argtable2

The argtable-2.4.tar.gz file contains the argtable2 distribution.

1.    Copy the argtable-2.4.tar.gz file to the location were you would like to build the CDF installation.

    cp argtable-2.4.tar.gz /user_specified_path

2.    Uncompress the distribution

    gzip -d argtable-2.4.tar.gz

3.    "Un-Tar" the tar file

    tar -x -v -f argtable-2.4.tar

4.    cd into the newly created argtable2 directory

    cd argtable2

5.    Run the configure script

    ./configure

6.    Compile the distribution

    make

7.    Test the Compilation ( Optional )

    make check

---------------------------------------------------------------------------------

Summary of how to install netCDF

The netcdf.tar.gz file contains the netCDF distribution

1.    Copy the netcdf.tar.gz file to the location were you would like to build the netCDF installation.

    cp netcdf.tar.gz /user_specified_path

2.    Uncompress the distribution

    gunzip netcdf.tar.gz

3.    "Un-Tar" the tar file

    tar -x -v -f netcdf.tar

4.    cd into the newly created netcdf-3-6.0-p1/src directory

    cd netcdf-3-6.0-p1/src

5.    Run the configure script

    ./configure


  NOTE: The configure script will try to find necessary tools in your path.
  When you run configure you may optionally use the --prefix argument to
  change the default installation directory.  The default is the distribution root ( netcdf-XXX/ )

    ie.     ./configure --prefix=/usr/local


6.    Compile the distribution

    make test

7.    Test the Compilation ( Optional )

    make install


------------------------------------------------------------------------------------

Keep note of the locations of both libcdf.a & libargtable2.a as they will be used for the installation of the ccmc-converter & ccmc-interpolater.

------------------------------------------------------------------------------------

CONTACT:

Marlo Maddox
Community Coordinated Modeling Center
NASA Goddard Space Flight Center
ISD - Advanced Data Management & Analysis Branch
Phone: 301.286.5202
email: Marlo.M.Maddox@nasa.gov