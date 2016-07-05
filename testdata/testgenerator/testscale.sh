#!/bin/bash
for image in "Lenna Lenna.png .png" "Tulips tulips.png .png" "Sparrow sparrow.jpg .jpg"
do
	imageArray=($image)
	echo "Processing: " ${imageArray[1]}
	for size in 50 75 125 150 200
	do
		magick ${imageArray[1]} -resize "$size"% "${imageArray[0]}"S"$size""${imageArray[2]}"
		echo -n "+++"
	done
	echo ""
done