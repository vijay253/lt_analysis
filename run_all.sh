#! /bin/bash

#
# Description:
# ================================================================
# Time-stamp: "2023-01-05 08:58:54 trottar"
# ================================================================
#
# Author:  Richard L. Trotta III <trotta@cua.edu>
#
# Copyright (c) trottar
#

SIMCPATH="/u/group/c-kaonlt/USERS/${USER}/simc_gfortran"

cd "${SIMCPATH}"

#./set_ProdBin.sh -a $1 $2 $3 $4 $5

#./set_ProdBin.sh -a high 5p5 3p02 5 lh2;./set_ProdBin.sh -a low 5p5 3p02 5 lh2;./set_ProdBin.sh -a high 4p4 2p74 5 lh2;./set_ProdBin.sh -a low 4p4 2p74 5 lh2;./set_ProdBin.sh -a high 3p0 3p14 5 lh2;./set_ProdBin.sh -a low 3p0 3p14 5 lh2;./set_ProdBin.sh -a high 3p0 2p32 5 lh2;./set_ProdBin.sh -a low 3p0 2p32 5 lh2;./set_ProdBin.sh -a high 2p1 2p95 5 lh2;./set_ProdBin.sh -a low 2p1 2p95 5 lh2;./set_ProdBin.sh -a high 0p5 2p40 5 lh2;./set_ProdBin.sh -a low 0p5 2p40 5 lh2;./set_ProdBin.sh -a high 5p5 3p02 5 dummy;./set_ProdBin.sh -a low 5p5 3p02 5 dummy;./set_ProdBin.sh -a high 4p4 2p74 5 dummy;./set_ProdBin.sh -a low 4p4 2p74 5 dummy;./set_ProdBin.sh -a high 3p0 3p14 5 dummy;./set_ProdBin.sh -a low 3p0 3p14 5 dummy;./set_ProdBin.sh -a high 3p0 2p32 5 dummy;./set_ProdBin.sh -a low 3p0 2p32 5 dummy;./set_ProdBin.sh -a high 2p1 2p95 5 dummy;./set_ProdBin.sh -a low 2p1 2p95 5 dummy;./set_ProdBin.sh -a high 0p5 2p40 5 dummy;./set_ProdBin.sh -a low 0p5 2p40 5 dummy


#batch_submit ProdSetup high 5p5 3p02 5 lh2;batch_submit ProdSetup low 5p5 3p02 5 lh2;batch_submit ProdSetup high 4p4 2p74 5 lh2;batch_submit ProdSetup low 4p4 2p74 5 lh2;batch_submit ProdSetup high 3p0 3p14 5 lh2;batch_submit ProdSetup low 3p0 3p14 5 lh2;batch_submit ProdSetup high 3p0 2p32 5 lh2;batch_submit ProdSetup low 3p0 2p32 5 lh2;batch_submit ProdSetup high 2p1 2p95 5 lh2;batch_submit ProdSetup low 2p1 2p95 5 lh2;batch_submit ProdSetup high 0p5 2p40 5 lh2;batch_submit ProdSetup low 0p5 2p40 5 lh2;batch_submit ProdSetup high 5p5 3p02 5 dummy;batch_submit ProdSetup low 5p5 3p02 5 dummy;batch_submit ProdSetup high 4p4 2p74 5 dummy;batch_submit ProdSetup low 4p4 2p74 5 dummy;batch_submit ProdSetup high 3p0 3p14 5 dummy;batch_submit ProdSetup low 3p0 3p14 5 dummy;batch_submit ProdSetup high 3p0 2p32 5 dummy;batch_submit ProdSetup low 3p0 2p32 5 dummy;batch_submit ProdSetup high 2p1 2p95 5 dummy;batch_submit ProdSetup low 2p1 2p95 5 dummy;batch_submit ProdSetup high 0p5 2p40 5 dummy;batch_submit ProdSetup low 0p5 2p40 5 dummy

./set_ProdBin.sh -a high 2p1 2p95 5 lh2;./set_ProdBin.sh -a low 2p1 2p95 5 lh2;
