let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/archive/63dacb46bf939521bdc93981b4cbb7ecb58427a0.tar.gz"; # 24.05
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
    pkgs.temurin-jre-bin-18
  ];

  shellHook = ''
    echo -e "\n\033[34mWelcome to \033[1mfilc\033[0m\033[34m dev environment\033[0m\n"
  '';
}
