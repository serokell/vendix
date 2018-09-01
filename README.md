```sh
$ nix-build
$ mkdir -p /tmp/nix/store/hello
$ DYLD_INSERT_LIBRARIES=result/lib/libnixvendor.dylib DYLD_ROOT_PATH=/tmp ls /nix/store
hello
```
