#!/bin/bash
for image in "Lenna Lenna.png .png" "Tulips tulips.png .png" "Sparrow sparrow.jpg .jpg"
do
	for size in 50 75 125 150 200
	do
		for noise in "Gaussian G" "Poisson P" "Random R"
		do
			for atten in 0.5 1.0
			do
				for angle in 45 90
				do
					noiseArray=($noise)
					imageArray=($image)
					magick ${imageArray[1]} -resize "$size"% +noise "${noiseArray[0]}" -attenuate "$atten" -rotate "$angle" "${imageArray[0]}"S"$size"N"${noiseArray[1]}""$atten"R"$angle""${imageArray[2]}"
				done
			done
		done
	done
done