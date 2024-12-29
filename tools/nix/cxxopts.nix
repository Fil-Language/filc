{ stdenv
, fetchFromGitHub
, icu
, cmake, pkg-config
}:

# From https://github.com/NixOS/nixpkgs/blob/nixos-24.11/pkgs/by-name/cx/cxxopts/package.nix
stdenv.mkDerivation rec {
  pname = "cxxopts";
  version = "3.2.0";

  src = fetchFromGitHub {
    owner = "jarro2783";
    repo = "cxxopts";
    tag = "v${version}";
    hash = "sha256-tOO0YCIG3MxSJZhurNcDR1pWIUEO/Har9mrCrZs3iVk=";
  };

#  buildInputs = [ icu.dev ];
  cmakeFlags = [
    "-DCXXOPTS_BUILD_EXAMPLES=OFF"
    "-DCXXOPTS_USE_UNICODE_HELP=FALSE"
  ];
  nativeBuildInputs = [ cmake pkg-config ];

  doCheck = true;
  dontUserCmakeBuildDir = true;

  postPatch = ''
    substituteInPlace packaging/pkgconfig.pc.in \
      --replace '$'{prefix}/@CMAKE_INSTALL_INCLUDEDIR@ @CMAKE_INSTALL_FULL_INCLUDEDIR@
  '';
}
