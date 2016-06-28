Setup Project
=============
1. Install Qt framework and Qt creator for development
	Get Qt creator from software center

2. Install ImageMagick
	Follow steps mentioned below

3. Configure GNUPG
	https://www.digitalocean.com/community/tutorials/how-to-use-gpg-to-encrypt-and-sign-messages-on-an-ubuntu-12-04-vps

4. Clone project from github
	https://github.com/Secure-Hash/blockhash.git

Install Magick++ library
========================
	1. Download souce code https://github.com/ImageMagick/ImageMagick
	2. Open terminal and move to root directory of source
	3. Configure build environment
	   $./configure
	4. Build and install
	   $ make
       $ sudo make install
    5. Test for proper installation
	   $make check
	6. Register shared objects
	   sudo ldconfig
	
How to compile programs for Magick++
====================================
	$ c++ -o output input.cppp `Magick++-config --cppflags --cxxflags --ldflags --libs`
