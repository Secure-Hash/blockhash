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
	
How to compile
==============
	$ c++ -o output input.cppp `Magick++-config --cppflags --cxxflags --ldflags --libs`
