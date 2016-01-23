#
#  QCustomPlot Plot Examples
#

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = plot-examples
TEMPLATE = app

SOURCES += main.cpp\
           mainwindow.cpp \
         qcustomplot.cpp \
    computemin.cpp \
    form.cpp \
    linearea.cpp \
    line.cpp \
    circle.cpp \
    sector.cpp \
    alglib3.9.0/src/alglibinternal.cpp \
    alglib3.9.0/src/alglibmisc.cpp \
    alglib3.9.0/src/ap.cpp \
    alglib3.9.0/src/dataanalysis.cpp \
    alglib3.9.0/src/diffequations.cpp \
    alglib3.9.0/src/fasttransforms.cpp \
    alglib3.9.0/src/integration.cpp \
    alglib3.9.0/src/interpolation.cpp \
    alglib3.9.0/src/linalg.cpp \
    alglib3.9.0/src/optimization.cpp \
    alglib3.9.0/src/solvers.cpp \
    alglib3.9.0/src/specialfunctions.cpp \
    alglib3.9.0/src/statistics.cpp \
    circlearea.cpp \
    Banking.cpp \
    dataProvide.cpp \
    area.cpp \
    Mark.cpp \
HEADERS  += mainwindow.h \
    qcustomplot.h \
    computemin.h \
    form.h \
    linearea.h \
    line.h \
    circle.h \
    sector.h \
    alglib3.9.0/src/alglibinternal.h \
    alglib3.9.0/src/alglibmisc.h \
    alglib3.9.0/src/ap.h \
    alglib3.9.0/src/dataanalysis.h \
    alglib3.9.0/src/diffequations.h \
    alglib3.9.0/src/fasttransforms.h \
    alglib3.9.0/src/integration.h \
    alglib3.9.0/src/interpolation.h \
    alglib3.9.0/src/linalg.h \
    alglib3.9.0/src/optimization.h \
    alglib3.9.0/src/solvers.h \
    alglib3.9.0/src/specialfunctions.h \
    alglib3.9.0/src/statistics.h \
    alglib3.9.0/src/stdafx.h \
    circlearea.h \
    Banking.h \
    nlopt.h \
    area.h \
	entropy.h \
	Mark.h

FORMS    += mainwindow.ui \
    form.ui

LIBS     += libnlopt-0.lib

