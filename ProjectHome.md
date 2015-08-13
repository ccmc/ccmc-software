# Kameleon Software Suite #

## Overview ##

Kameleon is a software suite that is being developed at the CCMC to address the difficulty in analyzing and disseminating the varying output formats of space weather model data.  Through the employment of a comprehensive data format standardization methodology, Kameleon allows heterogeneous model output to be stored uniformly in a common science data format.  The converted files contain both the original model output as well as additional metadata elements to create platform independent and self-descriptive data files.  To facilitate model data dissemination, data reuse, and code reuse – the Kameleon access and interpolation library provides direct access to both the model data as well as the embedded metadata.

In a nutshell, the Kameleon converter reads model output data and re-writes the data into a standard format.  The Kameleon access/interpolation library reads data already converted by Kameleon providing a high level interface to the data.

Current Feature/Functionality Chart: https://docs.google.com/spreadsheet/ccc?key=0AnFF96hg0HaldHhPVk9tUmtzZFBVdWF5VGNOaUxta3c&usp=sharing#gid=0

CCMC Software Developers Google Group:
https://groups.google.com/d/forum/ccmc-software

CCMC Software Developers Google Group Mail List: ccmc-software@googlegroups.com

## Where To Start?  What Are All Of These Versions? ##

The Kameleon Software Suite began life as two packages written in C.  The **Kameleon Converter Package** and the **Kameleon Access/Interpolation Library**.  Both converter and interpolator packages evolved, and were eventually re-written with successor distributions now in JAVA and C++ respectively.

The suite now consists of:

  1. Original **Kameleon Converter** ( C )
  1. Generation 2 **KameleonJ/Kameleon6 Converter** ( JAVA )
  1. Original **Kameleon Interpolator** ( C )
  1. Generation 2 **Kameleon+ Interpolator** ( C++ )


### For Developers ###

  * If you are a developer and want to **add new model** conversion functionality to the Kameleon Converter, you can use either the C or JAVA version depending on what language you are most comfortable with.  The CCMC is **investing primarily in KameleonJ/Kameleon6 development**, but we are not opposed to new external development activities/branches that extend the utility, features, functionality of the original version.

  * If you have model(s) and/or data and are **interested in writing directly** to the Kameleon supported format, please make sure all standard global and variable meta-data is created and compatible data structures are used.  _Please note that a formal specification document is currently under development for this..._

  * If you want to add new access/interpolation support to Kameleon, we recommend development using the Kameleon+ (C++) version of the library as a baseline.  The Kameleon+ version is expected to receive the bulk of both CCMC and community support

### For Users ###

  * If you already have Kameleon compatible data and want to read it, you should first verify that the KameleonJ/Kameleo6 version supports the model output you have.  If it does, we recommend you use the KameleonJ/Kameleo6 version.  However, if the original Kameleon access/interpolation library is the only version that supports the data you have and/or you are already comfortable with the original version, please feel free to use the original distribution.

Please note that the CCMC is **investing primarily in the new generations of KameleonJ/Kameleon6 and Kameleon+**.