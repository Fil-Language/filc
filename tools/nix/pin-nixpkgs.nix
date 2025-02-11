{}:

let
  pinNixpkgs = import (fetchTarball {
    url = "https://github.com/NixOS/nixpkgs/archive/5cb027673efd44307525d6300e0fa6309dce47d3.tar.gz";
    sha256 = "0qavs0z6djxa52hnxix0ndhl39yv1k6lj7x86xpjdgr078lhcy7a";
  } ) { config = {}; overlays = []; };
in pinNixpkgs
