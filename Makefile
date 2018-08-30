libnixvendor.dylib:
	cc -dynamiclib -Wno-gcc-compat -flat_namespace nix-vendor.c -o libnixvendor.dylib
