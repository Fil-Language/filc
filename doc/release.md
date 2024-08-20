# Release

This file is reminder for integrators on how to release a new filc version.

First of all, only a filc integrator is allowed to release a new version.

## Go on top of master branch

```shell
git checkout master
git fetch origin master
git pull
```

## Update version

Let's say we are releasing version `vX.Y.Z`.

```shell
./bin/generate_version.sh X.Y.Z
```

## Update the changelog

Get the current version changelog by generating the release note in GitHub release (but don't create the release!).

Add it in CHANGELOG.md under the `## vX.Y.Z` header. Also add in italic between the header and the list the release date.

## Commit

Just commit, the tag will be created by GitHub.

```shell
git add .
git commit -m "chore(release): Prepare v$(cat VERSION) release"
git push origin master
```

## Create the release on GitHub

Create the release with tag `vX.Y.Z`, use <kbd>Generate release notes</kbd> button. Check the set as latest release checkbox.

We're done!
