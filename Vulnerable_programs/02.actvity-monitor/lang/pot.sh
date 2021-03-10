#!/bin/bash

source pot.conf

# Create the POT files for Broadcast

cd ..

# Collect a list of all PHP files
PHPFILES=""
# This is an array of files to ignore.
IGNORE=( )
# Find all PHP files
for file in `find ./ -type f -name "*php"` ; do
	match=$(echo "${IGNORE[@]:0}" | grep -o $file)
	if [ "$match" == "" ]; then
		echo Will parse $file
		PHPFILES="$PHPFILES $file"
	fi
done

xgettext -s -c --no-wrap -d $POTDOMAIN -p lang -o "$POTFILE" \
	--omit-header -k_ -kerror_ -kdescription_ -kmessage_ -kheading_ -klabel_ -kname_ -koption_ -kp_ -ktext_ -kvalue_ -ktitle_ \
	$PHPFILES

# Run any custom pot scripts that need to be run.
lang/pot.*.sh
