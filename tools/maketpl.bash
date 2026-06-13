#!/usr/bin/env bash

errcount=0

trap '(( $? && ++errcount ))' ERR

rm -f *.png-tmp *.tpl-tmp
for t in "$@"; do
	echo "file: '$t'"

	if [[ ! "$t" =~ .+-([^.]+)\.png$ ]]; then
		echo "Skipping '$t': Does not match 'filename-format.png' pattern."
		continue
	fi

	format="${BASH_REMATCH[1]}"
	base="${t%%-$format.png}"
	target_tpl="$base.tpl"

	echo "step 1: oxipng"
	oxipng "$t" -aqomax --strip all --scale16
	if [ "$?" -ne "0" ]; then continue; fi

	echo "step 2: pngquant"
	pngquant --speed 1 --strip "$t" -fq --ext .png-tmp
	if [ "$?" -ne "0" ]; then continue; fi

	echo "step 3: oxipng (again)"
	oxipng "$t-tmp" -aqomax --strip all --scale16
	if [ "$?" -ne "0" ]; then continue; fi

	size1=$(wc -c <"$t-tmp")
	size2=$(wc -c <"$t")
	diff=$((size1 - size2))
	awk "BEGIN {printf \"\n%+.2f%% ($diff bytes) difference\n\n\", ($diff / $size2) * 100}"

	local_png="$t"
	if [ $size1 -ge $size2 ]; then
		echo "$t-tmp >= original file size, discarding..."
		rm -f "$t-tmp"
	else
		echo "$t-tmp is smaller!! replacing original :D"
		mv -f "$t-tmp" "$t"
	fi

	echo "step 4: wimgt encode"
	wimgt encode "$t" -oD "$target_tpl" -x "$format"
	if [ "$?" -ne "0" ]; then continue; fi

	echo
done

if [ "$errcount" -ne "0" ]; then
	echo "$errcount files failed"
else
	echo "success :)"
fi
