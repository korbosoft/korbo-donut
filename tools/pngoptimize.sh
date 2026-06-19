#!/usr/bin/env sh
errcount=0
for f in *.png; do
	starterrcount=$errcount
	rm -f *.png-tmp
	echo "file: '$f'"
	echo ""
	echo "step 1: oxipng"
	oxipng "$f" -aqomax --strip all --scale16 || errcount=$(($errcount + 1))
	if [ "$errcount" -eq "$starterrcount" ]; then
		echo ""
		echo "step 2: pngquant"
		pngquant "$f" --speed 1 -f --ext ".png-tmp" || errcount=$(($errcount + 1))
	fi
	if [ "$errcount" -eq "$starterrcount" ]; then
		echo ""
		echo "step 3: oxipng (again)"
		oxipng "$f" -aqomax --strip all --scale16 || errcount=$(($errcount + 1))
	fi
	if [ "$errcount" -eq "$starterrcount" ]; then
		echo ""
		size1=$(wc -c <"$f-tmp")
		size2=$(wc -c <"$f")
		diff=$(($size1 - $size2))
		awk "BEGIN {printf \"\n%+.2f%% ($diff bytes) difference\n\n\", ($diff / $size2) * 100}"
		if [ $size1 -ge $size2 ]; then
			echo "$f-tmp >= original file size, discarding..."
			rm -f "$f-tmp"
		else
			echo "$f-tmp is smaller!! replacing original :D"
			mv -f "$f-tmp" "$f"
		fi
		echo ""
	fi
done
if [ "$errcount" -ne "0" ]; then
	echo "optimization failed for $f"
else
	echo "success :)"
fi
