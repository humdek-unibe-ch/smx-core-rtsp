###############################################################################
# CUTOMIZE THIS FILE TO SUIT YOUR NEEDS FOR THE BUILD PROCESS                 #
# This file is included into the following files (refer to them for more      #
# information on the build process):                                          #
#  - Makefile                                                                 #
#  - test/Makefile                                                            #
###############################################################################

# The version number of the app ($(VMAJ).$(VMIN).$(VREV))
VMAJ = 1
VMIN = 1
VREV = 1
VDEB = 1

# the utility library
LIB_SMXUTILS = -lsmxutils-1.1

# the include path of the smxutlis header files
INC_SMXUTILS = -I/usr/include/smx/libsmxutils-1.1

# the name of the application
APPNAME = smxrtsp
