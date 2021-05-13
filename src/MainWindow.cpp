/*
 * @file MainWindow.cpp
 * @author Robert Borzecki (robert.borzecki@mudita.com)
 * @date 5 cze 2019
 * @brief
 * @copyright Copyright (C) 2019 mudita.com
 * @details
 */
#include <string.h>
#include <iomanip>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <gdkmm/pixbuf.h>

#include "VPIConverter.h"
#include "MPIConverter.h"
#include "MainWindow.h"

extern const uint32_t COLOUR_FOR_TRANSPARENT;
// Constructor of the main Window (build ui interface).
MainWindow::MainWindow( std::string filename, uint32_t alphaColor, bool blackWhite ) : Window()
{
	// Initialize the main window and hide the title bar
	this->set_border_width(10);

	Glib::RefPtr<Gdk::Pixbuf> piximage = Gdk::Pixbuf::create_from_file(filename);
	uint32_t pixOffset = 0;
	uint32_t offset = 0;
	uint8_t* pixBuffer = piximage.get()->get_pixels();
	uint32_t pixRowstride = piximage.get()->get_rowstride();
	uint32_t pixWidth = piximage.get()->get_width();
	uint32_t pixHeight = piximage.get()->get_height();
	uint32_t pixPixelSize = piximage.get()->get_n_channels();
        bool pixHasAlpha = piximage.get()->get_has_alpha();

	std::cout<<"rowstride: "<<pixRowstride<<std::endl;
	std::cout<<"width: "<<pixWidth<<std::endl;
	std::cout<<"height: "<<pixHeight<<std::endl;
	std::cout<<"bytes per pixel: "<<pixPixelSize<<std::endl;
	std::cout<<"black&white: "<<(blackWhite?"true":"false")<<std::endl;
	std::cout<<"has alpha? : "<<(pixHasAlpha?"true":"false")<<std::endl;


	//convert to byte per pixel representation
	uint32_t pixel = 0;
	uint8_t* buffer = new uint8_t[pixWidth*pixHeight];
	memset(buffer, 0, pixWidth*pixHeight );

	uint8_t pixel8;
	for( uint32_t y=0; y<pixHeight; ++y ) {
        pixOffset = y * pixRowstride; // in case of `pixPixelSize` and `pixWidth` being both odd, there is at least one additional byte at the end of each row that should be discarded. Probable cause: uintX_t (uint16_t?) -> uint8_t array conversion in gdkmm-3.0 lib
		for( uint32_t x=0; x<pixWidth; ++x ) {
			pixel8 = static_cast<uint8_t>((*(pixBuffer + pixOffset)) & 0x000000FF); // luminosity as a single byte. this whole cast is actually not needed, as pixBuffer is uint8_t. Here reading actually red channel
                  bool transparent = false;
                  if (pixHasAlpha) {
                    // then the last channel is alpha
                    uint8_t alpha_offset = pixPixelSize - 1;
                    uint8_t alpha8 = *(pixBuffer + pixOffset + alpha_offset);
                    transparent = alpha8 != 255 ? true : false;
                  }

                  if (transparent) {
                    pixel8 = COLOUR_FOR_TRANSPARENT;
                  } else {
                    pixel8 >>= 4;
                    if (blackWhite) {
                      if (pixel8 > 7)
                        pixel8 = 15;
                      else
                        pixel8 = 0;
                    }
                  }
                  buffer[offset] = pixel8;
			++offset;
			pixOffset+=pixPixelSize;
		}
	}

	uint32_t blackPixels;
	offset = 0;
	for( uint32_t y=0; y<pixHeight; ++y )
	{
		blackPixels = 0;
		for( uint32_t x=0; x<pixWidth; ++x ){

			pixel8 = buffer[offset];
			if( pixel8 == 0 )
				++blackPixels;
			++offset;
		}
		if( blackPixels ) {
//			std::cout<<"row: "<<y<<" black pixels: "<<blackPixels<<std::endl;
		}
	}

	VPIConverter::exportVPI( filename, buffer, pixWidth, pixHeight, alphaColor );


	memset(buffer, 0, pixWidth*pixHeight );
	offset = 0;
	pixOffset = 0;
	for( uint32_t y=0; y<pixHeight; ++y ) {
		for( uint32_t x=0; x<pixWidth; ++x ) {
			pixel8 = static_cast<uint8_t>((*(pixBuffer + pixOffset)) & 0x000000FF);
			pixel8 >>= 4;
			buffer[offset] = pixel8;
			++offset;
			pixOffset+=pixPixelSize;
		}
	}

	MPIConverter::exportMPI( filename, buffer, pixWidth, pixHeight );

	delete []buffer;


	// Load and display the image
	image.set(filename);
	mainGrid.attach(image,0,0,1,1);

	// Add the Quit button
	buttonQuit.add_label("Quit");
	buttonQuit.signal_pressed().connect(sigc::mem_fun(*this,&MainWindow::close));
	mainGrid.attach(buttonQuit,0,2,1,1);

	// Display the main grid in the main window
	mainGrid.show_all();

	// Insert the grid in the main window
	add(mainGrid);
}


// Destructor of the class
MainWindow::~MainWindow()
{}
