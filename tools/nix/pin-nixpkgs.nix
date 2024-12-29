{}:

let
  pinNixpkgs = import (fetchTarball {
    url = "https://github.com/NixOS/nixpkgs/archive/3e889463e499537c602e3ea83da6e33f9dc974da.tar.gz";
    sha256 = "1w7hilh5ix6vppxh5c2zd71l4k1y5wsx5h5s4jya3vw2hhhr99v6";
  } ) { config = {}; overlays = []; };
in pinNixpkgs
