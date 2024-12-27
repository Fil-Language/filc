let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/archive/3e889463e499537c602e3ea83da6e33f9dc974da.tar.gz";
  pkgs = import nixpkgs { config = {}; overlays = []; };
in

let
  pnpm = pkgs.callPackage ./tools/nix/pnpm.nix { nodejs = pkgs.nodejs_20; };
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
    pkgs.llvmPackages_18.libllvm
    pkgs.libffi
    pkgs.libxml2
  ];

  shellHook = ''
    pnpm install;
    echo -e "\n\033[34mWelcome to \033[1mfilc\033[0m\033[34m dev environment\033[0m\n"
  '';
}
