#!/bin/bash
for image in "Lenna Lenna.png .png" "Tulips tulips.png .png" "Sparrow sparrow.jpg .jpg"
do
	imageArray=($image)
	echo "Processing: " ${imageArray[1]}
	for noise in "Gaussian G" "Poisson P" "Random R"
	do
		noiseArray=($noise)
		for atten in 0.5 1.0
		do
			magick ${imageArray[1]} +noise "${noiseArray[0]}" -attenuate "$atten" "${imageArray[0]}"N"${noiseArray[1]}""$atten""${imageArray[2]}"
			echo -n "++"
		done
	done
	echo ""
done