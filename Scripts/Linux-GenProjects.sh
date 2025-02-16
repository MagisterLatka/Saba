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
cmake . --preset $option
echo "${option}" > .cmake-option
popd > /dev/null
