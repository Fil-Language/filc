#!/usr/bin/env bash

set -euo pipefail

if [[ -z "$(git status --porcelain)" ]]; then
  exit 0
else
  git diff
  exit 1
fi