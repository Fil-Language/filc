{ stdenvNoCC
, fetchurl
, nodejs
}:

# From https://github.com/NixOS/nixpkgs/blob/nixos-24.11/pkgs/development/tools/pnpm/generic.nix
stdenvNoCC.mkDerivation (finalAttrs: {
  pname = "pnpm";
  version = "9.15.1";

  src = fetchurl {
    url = "https://registry.npmjs.org/pnpm/-/pnpm-${finalAttrs.version}.tgz";
    hash = "sha256-nlNOcK/vBjdPYSa0S9pXYJRxNc4WowrvEBDpZft+Pj4=";
  };
  # Remove binary files from src, we don't need them, and this way we make sure
  # our distribution is free of binaryNativeCode
  preConfigure = ''
    rm -r dist/reflink.*node dist/vendor
  '';

  buildInputs = [ nodejs ];

  installPhase = ''
    runHook preInstall

    install -d $out/{bin,libexec}
    cp -R . $out/libexec/pnpm
    ln -s $out/libexec/pnpm/bin/pnpm.cjs $out/bin/pnpm
    ln -s $out/libexec/pnpm/bin/pnpx.cjs $out/bin/pnpx

    runHook postInstall
  '';
})
