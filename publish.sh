#!/bin/bash

outdir=ep3-thiago-wilson

if [ ! -d $outdir ]; then
  mkdir $outdir
fi

contents="src/ Makefile objs.makefile deps.makefile LEIAME sample_network"
cp -r $contents $outdir

tar -caf $outdir.tar.gz $outdir

rm -rf $outdir

