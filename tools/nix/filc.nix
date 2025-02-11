{ stdenvNoCC
, fetchFromGitHub
, cmake
, ninja
, clang
, cxxopts
, antlr4_13
, llvmPackages_18
}:

let
  cxxopts_filc = cxxopts.override { enableUnicodeHelp = false; };
in

stdenvNoCC.mkDerivation rec {
  pname = "filc";
  version = "0.1.0";

  src = fetchFromGitHub {
    owner = "Fil-Language";
    repo = "filc";
    rev = "v${version}";
    hash = "sha256-MB6ABwxVYdJ4YxLHKNvDLrFUN6M/u2/vk1+x3rjktPU=";
  };

  nativeBuildInputs = [ cmake ninja clang ];
  buildInputs = [ cxxopts_filc antlr4_13 antlr4_13.runtime.cpp llvmPackages_18.libllvm ];
}
