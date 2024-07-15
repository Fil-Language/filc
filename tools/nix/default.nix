{ pkgs }:

[
  (import ./pinned-bazel.nix { inherit pkgs; })
  (import ./pinned-clang.nix { inherit pkgs; })
]
