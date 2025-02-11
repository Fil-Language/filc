let
  pkgs = import (fetchTarball {
    url = "https://github.com/NixOS/nixpkgs/archive/eb28b94bd14835836b539bc3854a6abf929876d4.tar.gz";
    sha256 = "1wwr70sw70ib0qdr0cf7d9g7czlib5zm8hg8xjll7lsind6cfvwi";
  }) { config = {}; overlays = []; };
in {
  filc = pkgs.callPackage ./filc.nix { };
}
