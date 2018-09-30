with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "nix-vendor";
  src = lib.cleanSource ./.;

  nativeBuildInputs = [ coreutils meson ninja ];

  doCheck = true;

  checkPhase = ''
    test "59hb78ss84bkgqvqyyn4z5s6qmlxpp57-hello" = "$(env \
      DYLD_INSERT_LIBRARIES=$PWD/libnixvendor.dylib \
      DYLD_ROOT_PATH=$PWD/../tests ls /nix/store)"
  '';
}
