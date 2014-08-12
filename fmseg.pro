## Generated by HaiQ on Mon Aug 11 2014

TEMPLATE =	app

OBJECTS_DIR = ./build
MOC_DIR = ./build
DESTDIR = ./

CONFIG += qt release console

HEADERS += fmsegview.h seg_update_mask.h fmsegwidget.h scanlistwidget.h
SOURCES += main.cpp fmsegview.cpp seg_update_mask.cpp fmsegwidget.cpp scanlistwidget.cpp
FORMS +=

# COMMON CODE ###################
COMMONCODE += mda utils
message($$(DEVCOMMON))
include($$(DEVCOMMON)/devcommon.pri)
#mda
HEADERS += mda.h array2d.h array3d.h array4d.h
SOURCES += mda.cpp array2d.cpp array3d.cpp array4d.cpp 
HEADERS += jniftiimage2.h
SOURCES += jniftiimage2.cpp
include($$(DEVCOMMON)/mda/jniftiimage.pri)
#utils
HEADERS += affinetransformation.h jmatrix.h textfile.h qwait.h
SOURCES += affinetransformation.cpp jmatrix.cpp textfile.cpp qwait.cpp
###################################