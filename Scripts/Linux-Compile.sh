log=0
case $1 in
    "-h"|"--help")
        echo "Compile the project with correct option"
        echo "Switches:"
        echo "\t-h / --help - display this list"
        echo "\t-l / --log - produce compile.log in bin-int folder"
        exit 1
        ;;
    "-l"|"--log")
        log=1
        ;;
esac

read -p "Choose preset: 1=unixlike-gcc-debug, 2=unixlike-gcc-release, 3=unixlike-clang-debug, 4=unixlike-clang-release [default=3]: " option

case $option in
    "1"|"unixlike-gcc-debug")
        option="unixlike-gcc-debug"
        ;;
    "2"|"unixlike-gcc-release")
        option="unixlike-gcc-release"
        ;;
    "3"|"unixlike-clang-debug")
        option="unixlike-clang-debug"
        ;;
    "4"|"unixlike-clang-release")
        option="unixlike-clang-release"
        ;;
    "")
        option="unixlike-clang-debug"
        ;;
    *)
        echo "Invaid option"
        exit 1
        ;;
esac

pushd $(dirname "$0")/../ > /dev/null
if [ ! -d bin-int/$option ]; then
    echo "Run Linux-GenProjects.sh with $option"
    exit 1
fi
cd bin-int/$option > /dev/null
NB_CORES=$(grep -c '^processor' /proc/cpuinfo)
if [ $log -eq 1 ]; then
    ninja -j${NB_CORES} -l${NB_CORES} 2>&1 | tee compile.log
else
    ninja -j${NB_CORES} -l${NB_CORES}
fi
cd ../../ > /dev/null
popd > /dev/null
