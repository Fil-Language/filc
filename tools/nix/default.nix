{ pkgs }:

[
  (import ./pinned-bazel.nix { inherit pkgs; })
]
