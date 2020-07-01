#! /usr/bin/make -f
# $Id$

default: clean

PDADD: PDADD.o ED.o debug.o IOdata.o preprocessing.o selection.o refinement.o
PDADD.phi: PDADD.o.phi ED.o.phi debug.o.phi IOdata.o.phi preprocessing.o.phi selection.o.phi refinement.o.phi
#bestmatchparallel.serial: bestmatchparallel-run.o bestmatchparallel.o dtw.o envelope.o lb.o norm.o profiler.o reader.o

#mktidbitmap: mktidbitmap.o tidbitmap.o csvparser.o

include $(ROOT)Makefile.Rules