with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "nix-vendor";
  src = lib.cleanSource ./.;

  nativeBuildInputs = [ meson ninja ];

  doCheck = true;

  preCheck = ''
    cp -r ../test/nix .
    ls -ahl
  '';
}
