let
  pkgs = import ./pin-nixpkgs.nix { };
in {
  filc = pkgs.callPackage ./filc.nix { };
}
