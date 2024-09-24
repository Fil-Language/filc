let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/archive/eb28b94bd14835836b539bc3854a6abf929876d4.tar.gz";
  pkgs = import nixpkgs { config = {}; overlays = []; };
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
    pkgs.corepack_20
  ];

  shellHook = ''
    pnpm install;
    echo -e "\n\033[34mWelcome to \033[1mfilc\033[0m\033[34m dev environment\033[0m\n"
  '';
}
