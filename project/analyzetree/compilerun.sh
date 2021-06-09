#!/bin/bash

PROJDIR=/home/masterdesky/GitHub/ELTE_High_Energy_2021/project

# Compile
cd ${PROJDIR}/analyzetree/src

make clean
make ../bin/analyzetree

cd ${PROJDIR}/analyzetree

echo
echo

# Run
${PROJDIR}/analyzetree/bin/analyzetree \
        ${PROJDIR}/data/data.root \
        ${PROJDIR}/data/analyzetree.root \
        ${PROJDIR}/data/reactionplane_resolution_rxnp.txt \
        $1 \
        40

root -b -q ''"${PROJDIR}"'/analyzetree/plot_analyzetree.C("'"${PROJDIR}"'/data/analyzetree.root","'"${PROJDIR}"'/figs")'