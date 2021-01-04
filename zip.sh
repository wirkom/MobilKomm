#!/bin/sh
rm -rf MoCo_WiTe_202021_PA_957634.zip || true
mkdir MoCo_WiTe_202021_PA_957634
cp PA_957634_* src scratch MoCo_WiTe_202021_PA_957634 -r
zip MoCo_WiTe_202021_PA_957634.zip MoCo_WiTe_202021_PA_957634 -r
rm MoCo_WiTe_202021_PA_957634 -rf
