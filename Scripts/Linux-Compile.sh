pushd $(dirname "$0")/../ > /dev/null
OPTION=$(cat .cmake-option)
cd bin-int/$OPTION > /dev/null
NB_CORES=$(grep -c '^processor' /proc/cpuinfo)
ninja -j${NB_CORES} -l${NB_CORES}
cd ../../
popd > /dev/null
