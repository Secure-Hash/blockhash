#!/bin/bash
for image in "Lenna Lenna.png .png" "Tulips tulips.png .png" "Sparrow sparrow.jpg .jpg"
do
	imageArray=($image)
	echo "Processing: " ${imageArray[1]}
	for angle in 10 15 30 40 45 50 60 75 85 90 57 88 17
	do
		magick ${imageArray[1]} -rotate "$angle"% "${imageArray[0]}"R"$angle""${imageArray[2]}"
		echo -n "+++"
	done
	echo ""
done