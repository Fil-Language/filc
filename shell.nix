{ pkgs ? (import ./tools/nix/pin-nixpkgs.nix) {} }:

let
  currentDir = builtins.toString ./.;
  pnpm = pkgs.pnpm.override { 
    version = "10.4.1";
    hash = "sha256-S3Aoh5hplZM9QwCDawTW0CpDvHK1Lk9+k6TKYIuVkZc=";
    nodejs = pkgs.nodejs_20;
  };
  cxxopts = pkgs.cxxopts.override { enableUnicodeHelp = false; };
  antlr4 = pkgs.callPackage ./tools/nix/antlr4.nix { };
in

pkgs.mkShell {
  name = "filc-dev-env";

  packages = [
    pkgs.git
    pkgs.cmake
    pkgs.ninja
    pkgs.lcov
    pkgs.libclang
    pkgs.valgrind
    pkgs.dpkg
    pkgs.rpm
    pkgs.jre_minimal
    pkgs.nodejs_20
    pnpm
    pkgs.httpie
    pkgs.llvmPackages_18.libllvm
    cxxopts
    antlr4.antlr
    antlr4.runtime.cpp
  ];

  shellHook = ''
    export ROOT_DIR="${currentDir}"
    export PATH="$PATH:${currentDir}/tools/bin"
    pnpm install;
    echo -e "\n\033[34mWelcome to \033[1mfilc\033[0m\033[34m dev environment\033[0m\n"
  '';
}
