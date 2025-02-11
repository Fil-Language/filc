{ pkgs ? (import ./tools/nix/pin-nixpkgs.nix) {} }:

let
  currentDir = builtins.toString ./.;
  pnpm = pkgs.callPackage ./tools/nix/pnpm.nix { nodejs = pkgs.nodejs_20; };
  cxxopts = pkgs.cxxopts.override { enableUnicodeHelp = false; };
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
    pkgs.libffi
    pkgs.libxml2
    cxxopts
    pkgs.antlr4_13
    pkgs.antlr4_13.runtime.cpp
  ];

  shellHook = ''
    export ROOT_DIR="${currentDir}"
    export PATH="$PATH:${currentDir}/tools/bin"
    pnpm install;
    echo -e "\n\033[34mWelcome to \033[1mfilc\033[0m\033[34m dev environment\033[0m\n"
  '';
}
