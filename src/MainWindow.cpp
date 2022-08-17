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


// calculate relative luminance for RGB colorspace: https://en.wikipedia.org/wiki/Relative_luminance
constexpr uint8_t calculate_rgb_luminance(uint8_t red, uint8_t green, uint8_t blue)
{
  return
      0.2126 * float(red) +
      0.7152 * float(green) +
      0.0722 * float(blue);
}

extern const uint32_t COLOUR_FOR_TRANSPARENT;
// Constructor of the main Window (build ui interface).
MainWindow::MainWindow( std::string filename, uint32_t alphaColor, bool blackWhite ) : Window()
{
	// Initialize the main window and hide the title bar
	this->set_border_width(10);

	Glib::RefPtr<Gdk::Pixbuf> piximage = Gdk::Pixbuf::create_from_file(filename);
  g_assert(piximage.get()->get_colorspace() == Gdk::COLORSPACE_RGB); // this could only be triggered if they change the API

	uint32_t pixOffset = 0;
  uint32_t offset = 0;
  uint8_t* const pixBuffer = piximage.get()->get_pixels();
  const uint32_t pixRowstride = piximage.get()->get_rowstride();
  const uint32_t pixWidth = piximage.get()->get_width();
  const uint32_t pixHeight = piximage.get()->get_height();
  const uint32_t pixPixelSize = piximage.get()->get_n_channels();
  const bool pixHasAlpha = piximage.get()->get_has_alpha();

	std::cout<<"rowstride: "<<pixRowstride<<std::endl;
	std::cout<<"width: "<<pixWidth<<std::endl;
	std::cout<<"height: "<<pixHeight<<std::endl;
	std::cout<<"bytes per pixel: "<<pixPixelSize<<std::endl;
	std::cout<<"black&white: "<<(blackWhite?"true":"false")<<std::endl;
	std::cout<<"has alpha? : "<<(pixHasAlpha?"true":"false")<<std::endl;


  constexpr uint32_t red_channel = 0;
  constexpr uint32_t green_channel = 1;
  constexpr uint32_t blue_channel = 2;

  bool grayscale_image = true;

  for( uint32_t y=0; y<pixHeight; ++y ) {
    pixOffset = y * pixRowstride; // in case of `pixPixelSize` and `pixWidth` being both odd, there is at least one additional byte at the end of each row that should be discarded. Probable cause: uintX_t (uint16_t?) -> uint8_t array conversion in gdkmm-3.0 lib
    for( uint32_t x=0; x<pixWidth; ++x ) {
      if(pixBuffer[pixOffset + red_channel] != pixBuffer[pixOffset + green_channel] || // all the channels are the same value
         pixBuffer[pixOffset + red_channel] != pixBuffer[pixOffset + blue_channel])
        grayscale_image = false;
      pixOffset+=pixPixelSize;
    }
  }

	//convert to byte per pixel representation
	uint32_t pixel = 0;
	uint8_t* buffer = new uint8_t[pixWidth*pixHeight];
	memset(buffer, 0, pixWidth*pixHeight );

	uint8_t pixel8;
	for( uint32_t y=0; y<pixHeight; ++y ) {
        pixOffset = y * pixRowstride; // in case of `pixPixelSize` and `pixWidth` being both odd, there is at least one additional byte at the end of each row that should be discarded. Probable cause: uintX_t (uint16_t?) -> uint8_t array conversion in gdkmm-3.0 lib
		for( uint32_t x=0; x<pixWidth; ++x ) {
      if(grayscale_image) // use value as luminance
        pixel8 = pixBuffer[pixOffset];
      else // calculate relative luminance from color: https://en.wikipedia.org/wiki/Relative_luminance
      {
        pixel8 = calculate_rgb_luminance(
                   pixBuffer[pixOffset + red_channel],
                   pixBuffer[pixOffset + green_channel],
                   pixBuffer[pixOffset + blue_channel]);
      }

                  bool transparent = false;
                  if (pixHasAlpha) {
                    // then the last channel is alpha
                    uint8_t alpha_offset = pixPixelSize - 1;
                    uint8_t alpha8 = *(pixBuffer + pixOffset + alpha_offset);
                    transparent = alpha8 != 255 ? true : false;
                  }

                  if (transparent) {
                    pixel8 = COLOUR_FOR_TRANSPARENT;
                  } else if (blackWhite) {
                    if (pixel8 & 0x80)
                      pixel8 = 15;
                    else
                      pixel8 = 0;
                  } else { // luminence scaling
                    constexpr double maxColorValue = 15.0;
                    constexpr double oldMaxColorValue = 255.0;
                    double val = pixel8;
                    val *= maxColorValue;
                    val += oldMaxColorValue / 2.0;
                    val /= oldMaxColorValue;
                    val = std::floor(val);
                    pixel8 = val;
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
