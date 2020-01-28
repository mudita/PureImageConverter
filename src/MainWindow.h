/*
 * @file MainWindow.h
 * @author Robert Borzecki (robert.borzecki@mudita.com)
 * @date 5 cze 2019
 * @brief
 * @copyright Copyright (C) 2019 mudita.com
 * @details
 */
#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <cstdint>
#include <string>
#include <gtkmm.h>

// The class mainwindow inherits from Gtk::Window
class MainWindow : public Gtk::Window
{
	// Constructor and destructor
public:
        static const uint32_t COLOUR_FOR_TRANSPARENT = 16; // 17th colour, as 0-15 are regular ones
	MainWindow( std::string filename, uint32_t alphaColor, bool blackWhite );
	virtual ~MainWindow();

	void exportVPI( std::string filename, uint8_t* buffer, uint32_t w, uint32_t h, uint32_t alpha );
protected:

	//Member widgets:
	Gtk::Image              image;
	Gtk::Button             buttonQuit;
	Gtk::Grid               mainGrid;
};

#endif /* MAINWINDOW_H_ */
