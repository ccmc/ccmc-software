rm -rf bin
mkdir bin
javac -cp lib/cdfjava.jar:lib/jargs.jar:lib/jhdf.jar:lib/jhdf5.jar:lib/jhdf5obj.jar:lib/jhdfobj.jar:lib/log4j-1.2.15.jar:lib/netcdf-4.2.jar:lib/nujan-1.4.1.1.jar:lib/slf4j-api-1.5.6.jar:lib/slf4j-log4j12-1.5.6.jar -d bin src/gov/nasa/gsfc/ccmc/KameleonConverter/*.java
echo "Run with the following command:"
echo "java -cp lib/cdfjava.jar:lib/jargs.jar:lib/jhdf.jar:lib/jhdf5.jar:lib/jhdf5obj.jar:lib/jhdfobj.jar:lib/log4j-1.2.15.jar:lib/netcdf-4.2.jar:lib/nujan-1.4.1.1.jar:lib/slf4j-api-1.5.6.jar:lib/slf4j-log4j12-1.5.6.jar:bin -Djava.library.path=lib gov.nasa.gsfc.ccmc.KameleonConverter.CommandLineInterface"

