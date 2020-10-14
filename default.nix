{ pkgs ? import <nixpkgs> { } }:

pkgs.stdenv.mkDerivation {
  name = "print-gcc-switches";
  src = ./.;
  nativeBuildInputs = with pkgs; [
    cmake
  ];
}
