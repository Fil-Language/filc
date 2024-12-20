#!/usr/bin/env bash

set -euo pipefail

# Get version in argument
if (($# != 1)); then
  echo -e "\033[31mUsage : $0 <new version>\033[0m"
  exit 1
fi
version=$1
major=$(echo "$version" | cut -d. -f1)
minor=$(echo "$version" | cut -d. -f2)
patch=$(echo "$version" | cut -d. -f3)
pre=$(echo "$patch" | cut -d- -f2 -s)
patch=$(echo "$patch" | cut -d- -f1)

# Check that command http is installed
if ! type "http" >/dev/null; then
  echo -e "\033[31mhttp command not found. Please install it.\033[0m"
  exit 1
fi

echo -e "\033[34mChange version to \033[1m$version\033[0m"

# Download badge for version
echo -e " -> Download badge"

version_badge=$version
if [ -n "$pre" ]
then
  version_badge="$major.$minor.$patch--$pre"
fi

echo -e "    https://img.shields.io/badge/version-$version_badge-green"
http --download --output=version.svg "https://img.shields.io/badge/version-$version_badge-green" -q
echo -e "    \033[32mDone\033[0m"

# Change version in VERSION
echo -e " -> Change version in VERSION"
echo -e -n "$version" > VERSION
echo -e "    \033[32mDone\033[0m"

# Version to change manually
echo -e " -> You must change version manually in some tests files"

