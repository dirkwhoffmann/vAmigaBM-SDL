// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "Multimedia.h"
#include "Layer.h"
#include "Interpreter.h"

#include <stack>
#include <sstream>
#include <string>
#include <vector>

class Console : public Layer {
    

    // Interpreter for commands typed into the debug console
    Interpreter interpreter = Interpreter(app);

    
    //
    // Constants
    //
  
    // The maximum number of rows
    // static const int maxRows = 40;
        
    // Layout properties
    mm::Font consoleFont;
    int padx, pady, lineSkip;
    int fontSize = 28;
    
    // Input prompt
    std::string prompt = "vAmiga\% ";
    
    //
    // Dimensions
    //
    
    // Number of text rows and text columns
    int numRows = 25;
    int numCols = 80;

    
    //
    // Text storage
    //
    
    // The text storage
    std::vector<std::string> storage;
    
    // The input history buffer
    std::vector<std::string> input;

    // The number of the first displayed line
    int vpos = 0;
    
    // The current cursor position
    int cpos = 0;
        
    // The currently active input string
    int ipos = 0;

    // Used to detect if the user presses TAB two times in a row
    int doubleTab = 0;
    
    // The render target
    mm::RenderView view;
    
    // The rendered text rows
    std::vector<mm::TextView> text;

    // Indicates if the render texture needs to be redrawn
    bool isDirty = true;
    
    // Font properties
    int glyphWidth = 0;
        
    // The cursor's visual shape
    mm::GradientView cursor;
                    
    
    //
    // Initializing
    //

public:

    Console(class Application &ref);
    ~Console();
    
    // Delegation methods
    void init();
    void configure();

    
    //
    // Performing continuous tasks
    //
    
    void update(i64 frame, mm::Time dt);
    void render();
    
    
    //
    // Responding to events
    //

    void respond(mm::Event &event);
    void respondToKeyboardEvent(mm::Event &event);
    void resize(int w, int h);
    
    
    //
    // Receiving notifications
    //
    
    void alphaDidChange() override;
    
    
    //
    // Opening and closing
    //
    
public:
    
    // Opens or closes the console
    void open() { setTargetAlpha(0xFF, 0.2); }
    void close() { setTargetAlpha(0x00, 0.2); }
    void toggle() { printf("toggle\n"); isVisible() ? close() : open(); }
 
 
    //
    // Adjusting the layout
    //
    
    void setNumRows(int value);
    void setNumCols(int value);

    
    //
    // Working with the text storage
    //
    
public:

    // Returns a reference to the last line in the text storage
    string &lastLine() { return storage.back(); }
            
    // Clears the console window
    void clear();
    
    // Prints a help line
    void printHelp();

    // Prints the input prompt
    void printPrompt();
        
    // Moves to a certain line in the console window
    void scrollTo(int line);
    void scrollToTop() { scrollTo(0); }
    void scrollUp(int delta) { scrollTo(vpos - delta); }
    void scrollDown(int delta) { scrollTo(vpos + delta); }
    void makeLastLineVisible();
    
    // Prints a message
    Console &operator<<(char value);
    Console &operator<<(const string &value);
    Console &operator<<(int value);
    Console &operator<<(long value);

    // Shortens the text storage if it grows too large
    void shorten();
    
    // Clears the current line
    void clearLine() { *this << '\r'; }

    // Moves the cursor forward to a certain column
    void tab(int hpos);

    // Replaces the last line
    void replace(const string &text, const string &prefix);
    void replace(const string &text) { replace(text, prompt); }

    // Prints some debug output
    void list();

    // Returns the row number of the last displayed line
    int rowOfLastLine();

    // Checks if the last line is visible
    bool lastLineIsVisible();
    
    
    //
    // Processing input
    //
    
public:
    
    // Processes a user typed character
    void type(char c);
    
    // Processes a mouse scrolling event
    void scroll(float delta);
    
    // Executes a user command
    bool exec(const string &command, bool verbose = false);
    
    // Executes a user script
    void exec(std::istream &stream) throws;

    
    //
    // Rendering
    //
        
private:

    // Translates rows and columns to drawing coordinates
    int hposForCol(int i) { return padx + i * glyphWidth; }
    int vposForRow(int i) { return pady + (fontSize + lineSkip) * i; }

    // Redraws the render texture
    void updateTexture();
};
