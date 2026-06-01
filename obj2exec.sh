 g++ build/obj/*.o \
  -L./vcpkg_installed/x64-linux/lib \
  -lcurl -lssl -lcrypto -lz \
  -o build/bin/projsilinstaller
