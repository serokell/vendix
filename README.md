```sh
$ make
$ mkdir -p /tmp/nix/store
$ touch /tmp/nix/store/hello
$ DYLD_INSERT_LIBRARIES=libnixvendor.dylib DYLD_FORCE_FLAT_NAMESPACE=1 BUNDLE_ROOT=/tmp ls /nix/store
hello
```
