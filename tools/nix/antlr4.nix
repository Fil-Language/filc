{ stdenv
, fetchurl
, fetchFromGitHub
, jre
, cmake, ninja, pkg-config
}:
# From https://github.com/NixOS/nixpkgs/blob/nixos-24.11/pkgs/development/tools/parsing/antlr/4.nix
let
  mkAntlr = {
    version, jarHash, srcHash
  }: rec {
    antlr = stdenv.mkDerivation {
      pname = "antlr";
      inherit version;
      src = fetchurl {
        url = "https://www.antlr.org/download/antlr-${version}-complete.jar";
        hash = jarHash;
      };
      dontUnpack = true;
      installPhase = ''
        mkdir -p "$out"/{share/java,bin}
        ln -s "$src" "$out/share/java/antlr-${version}-complete.jar"
        echo "#! ${stdenv.shell}" >> "$out/bin/antlr"
        echo "'${jre}/bin/java' -cp '$out/share/java/antlr-${version}-complete.jar:$CLASSPATH' -Xmx500M org.antlr.v4.Tool \"\$@\"" >> "$out/bin/antlr"
        echo "#! ${stdenv.shell}" >> "$out/bin/antlr-parse"
        echo "'${jre}/bin/java' -cp '$out/share/java/antlr-${version}-complete.jar:$CLASSPATH' -Xmx500M org.antlr.v4.gui.Interpreter \"\$@\"" >> "$out/bin/antlr-parse"
        echo "#! ${stdenv.shell}" >> "$out/bin/grun"
        echo "'${jre}/bin/java' -cp '$out/share/java/antlr-${version}-complete.jar:$CLASSPATH' org.antlr.v4.gui.TestRig \"\$@\"" >> "$out/bin/grun"
        chmod a+x "$out/bin/antlr" "$out/bin/antlr-parse" "$out/bin/grun"
        ln -s "$out/bin/antlr"{,4}
        ln -s "$out/bin/antlr"{,4}-parse
      '';
      inherit jre;
      passthru = {
        inherit runtime;
        jarLocation = antlr.src;
      };
    };
    runtime.cpp = stdenv.mkDerivation {
      pname = "antlr-runtime-cpp";
      inherit version;
      src = fetchFromGitHub {
        owner = "antlr";
        repo = "antlr4";
        rev = version;
        hash = srcHash;
      };
      outputs = [ "out" "dev" "doc" ];
      nativeBuildInputs = [ cmake ninja pkg-config ];
      cmakeDir = "../runtime/Cpp";
      cmakeFlags = [
        "-DANTLR4_INSTALL=ON"
        "-DANTLR_BUILD_CPP_TESTS=OFF"
      ];
    };
  };
in
mkAntlr {
  version = "4.13.2";
  jarHash = "sha256-6uLfoRmmQydERnKv9j6ew1ogGA3FuAkLemq4USXfTXY=";
  srcHash = "sha256-s1yAdScMYg1wFpYNsBAtpifIhQsnSAgJg7JjPDx+htc=";
}
