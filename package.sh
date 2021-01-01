#!/bin/bash

read -p "Input version number: " version
current_build="$(ls | grep autoinstall-manager_ | grep -v '.deb')"
build_name="autoinstall-manager_${version}_x86_64"
echo "Current build: ${current_build}"
echo "Packaging ${build_name}..."

# update version if applicable
if [ "$current_build" = "$build_name" ]; then
  echo "Warning, rebuilding existing version!"
else
  mv $current_build $build_name
fi

# build package
cp autoinstall-manager "${build_name}/usr/bin"
dpkg-deb --build $build_name
