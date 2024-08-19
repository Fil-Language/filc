#!/usr/bin/env bash

if (($# != 1)); then
  echo -e "\033[31mUsage : $0 <version>\033[0m"
  exit 1
fi
version=$1

# Check that command http is installed
if ! type "http" >/dev/null; then
  echo -e "\033[31mhttp command not found. Please install it.\033[0m"
  exit 1
fi

echo -e "\033[34mRemove previous jar\033[0m"
rm ./bin/antlr-*-complete.jar
echo -e "    \033[32mDone\033[0m"

echo -e "\033[34mDownload jar \033[1m$version\033[0m"
echo -e "    https://www.antlr.org/download/antlr-$version-complete.jar"
http --download --output="./bin/antlr-$version-complete.jar" "https://www.antlr.org/download/antlr-$version-complete.jar" -q
echo -e "    \033[32mDone\033[0m"
