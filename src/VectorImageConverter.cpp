//============================================================================
// Name        : pure-gui.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <gtkmm.h>
#include <gtkmm/application.h>

#include "MainWindow.h"


int main(int argc, char* argv[])
{
	uint32_t alphaColor = 15;
	std::string filename;
	bool blackWhite = false;
	if( argc == 2 ) {
		filename = argv[1];
	}
	if( argc == 3 ) {
		filename = argv[1];
		alphaColor = atoi( argv[2] );
	}
	if( argc == 4 ) {
		filename = argv[1];
		alphaColor = atoi( argv[2] );
		std::string flag = std::string(argv[3]);
		if( (flag == "Y") || (flag == "y") || (flag == "t") || (flag == "T"))
			blackWhite = true;
	}

	if( filename.empty()) {
		std::cout<<"No image file provided"<<std::endl;
		exit(0);
	}

	std::cout<< "file: "<<filename<<" transparentColor: "<<alphaColor<<std::endl;


	// Initialize gtkmm
	Gtk::Main app(argc, argv);
	// Create the window
	MainWindow w(filename, alphaColor, blackWhite );
	// Start main loop
	Gtk::Main::run(w);
	return 0;
}
