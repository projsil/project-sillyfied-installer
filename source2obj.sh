for f in projsilinstaller-win/*.cpp; do
  out=build/obj/$(basename "${f%.*}").o
  g++ -std=c++17 -fPIC -Wall -Wextra -O2 \
    -Iprojsilinstaller-win -I./vcpkg_installed/x64-linux/include \
    -c "$f" -o "$out"
done
