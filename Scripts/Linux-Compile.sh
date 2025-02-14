pushd $(dirname "$0")/../bin-int/ > /dev/null
NB_CORES=$(grep -c '^processor' /proc/cpuinfo)
ninja -j${NB_CORES} -l${NB_CORES}
popd > /dev/null
