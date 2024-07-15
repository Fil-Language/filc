let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/archive/53e81e790209e41f0c1efa9ff26ff2fd7ab35e27.tar.gz";
  pkgs = import nixpkgs { config = {}; overlays = []; };
in

pkgs.mkShell {
  packages = with pkgs; [];

  shellHook = ''
    echo -e "\n\033[34mWelcome to \033[1mfilc\033[0m\033[34m dev environment\033[0m\n"
  '';
}