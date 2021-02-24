// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "Command.h"
#include "Exception.h"
#include "Errors.h"

enum class Token
{
    none,
    
    // Components
    agnus, amiga, audio, blitter, cia, copper, cpu, denise, dfn, dc, keyboard,
    memory, mouse, paula, serial, rtc,

    // Commands
    about, audiate, autosync, clear, config, connect, disconnect, dsksync,
    easteregg, eject, close, insert, inspect, list, load, lock, on, off, pause,
    reset, run, set, source,
    
    // Categories
    state, registers, events, checksums,
    
    // Keys
    accuracy, bankmap, borderblank, chip, clxsprspr, clxsprplf, clxplfplf,
    device, esync, extrom, extstart, fast, filter, mechanics, model,
    palette, pan, poll, pullup, raminitpattern, revision, rom, sampling, slow,
    slowramdelay, slowrammirror, speed, step, tod, todbug, unmappingtype,
    velocity, volume, wom
};

struct ParseBoolError : public ParseError {
    using ParseError::ParseError;
};

struct TooFewArgumentsError : public ParseError {
    using ParseError::ParseError;
};

struct TooManyArgumentsError : public ParseError {
    using ParseError::ParseError;
};

class Interpreter : public GUIComponent
{    
    // The registered instruction set
    Command root;
    
    
    //
    // Initializing
    //

public:
    
    Interpreter(Application &ref);

private:
    
    // Registers the instruction set
    void registerInstructions();
    
    
    //
    // Parsing input
    //
    
public:
    
    // Splits an input string into an argument list
    Arguments split(const string& userInput);

    // Auto-completes a command. Returns the number of auto-completed tokens
    void autoComplete(Arguments &argv);
    void autoComplete(string& userInput);

    
    //
    // Executing commands
    //
    
public:
    
    // Executes a script file
    // void exec(std::istream &stream) throws;

    // Executes a single command
    void exec(const string& userInput, bool verbose = false) throws;
    void exec(Arguments &argv, bool verbose = false) throws;
            
    // Prints a usage string for a command
    void usage(Command &command);
    
    // Displays a help text for a (partially typed in) command
    void help(const string &userInput);
    void help(Arguments &argv);
    void help(Command &command);
};
