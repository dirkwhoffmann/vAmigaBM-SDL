// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"
#include "Amiga.h"

//
// Top-level commands
//

template <> void
Controller::exec <Token::clear> (Arguments &argv, long param)
{
    app.console.clear();
}

template <> void
Controller::exec <Token::close> (Arguments &argv, long param)
{
    app.console.close();
}

template <> void
Controller::exec <Token::easteregg> (Arguments& argv, long param)
{
    console << "GREETINGS PROFESSOR HOFFMANN." << '\n' << '\n';
    console << "THE ONLY WINNING MOVE IS NOT TO PLAY." << '\n' << '\n';
    console << "HOW ABOUT A NICE GAME OF CHESS?" << '\n';
}

template <> void
Controller::exec <Token::source> (Arguments &argv, long param)
{
    string filename = argv.front();
    
    std::ifstream stream(filename);
    if (!stream.is_open()) throw ConfigFileReadError(filename);
    
    try {
        app.console.exec(stream);
    } catch (Exception &e) {
        console << "Error in line " << (isize)e.data << '\n';
        console << e.what() << '\n';
    }
}

//
// Amiga
//

template <> void
Controller::exec <Token::amiga, Token::on> (Arguments &argv, long param)
{
    amiga.powerOn();
}

template <> void
Controller::exec <Token::amiga, Token::off> (Arguments &argv, long param)
{
    amiga.powerOff();
}

template <> void
Controller::exec <Token::amiga, Token::run> (Arguments &argv, long param)
{
    amiga.run();
}

template <> void
Controller::exec <Token::amiga, Token::pause> (Arguments &argv, long param)
{
    amiga.pause();
}

template <> void
Controller::exec <Token::amiga, Token::reset> (Arguments &argv, long param)
{
    amiga.reset(true);
}

template <> void
Controller::exec <Token::amiga, Token::inspect> (Arguments &argv, long param)
{
    dump(amiga, Dump::State);
}


//
// Memory
//

template <> void
Controller::exec <Token::memory, Token::config> (Arguments& argv, long param)
{
    dump(amiga.mem, Dump::Config);
}

template <> void
Controller::exec <Token::memory, Token::load, Token::rom> (Arguments& argv, long param)
{
    amiga.mem.loadRomFromFile(argv.front().c_str());
}

template <> void
Controller::exec <Token::memory, Token::load, Token::extrom> (Arguments& argv, long param)
{
    amiga.mem.loadExtFromFile(argv.front().c_str());
}

template <> void
Controller::exec <Token::memory, Token::set, Token::chip> (Arguments& argv, long param)
{
    amiga.configure(OPT_CHIP_RAM, parseDec(argv.front()));
}

template <> void
Controller::exec <Token::memory, Token::set, Token::slow> (Arguments& argv, long param)
{
    amiga.configure(OPT_SLOW_RAM, parseDec(argv.front()));
}

template <> void
Controller::exec <Token::memory, Token::set, Token::fast> (Arguments& argv, long param)
{
    amiga.configure(OPT_FAST_RAM, parseDec(argv.front()));
}

template <> void
Controller::exec <Token::memory, Token::set, Token::extstart> (Arguments& argv, long param)
{
    amiga.configure(OPT_EXT_START, parseHex(argv.front()));
}

template <> void
Controller::exec <Token::memory, Token::set, Token::slowramdelay> (Arguments& argv, long param)
{
    amiga.configure(OPT_SLOW_RAM_DELAY, parseBool(argv.front()));
}

template <> void
Controller::exec <Token::memory, Token::set, Token::bankmap> (Arguments& argv, long param)
{
    amiga.configure(OPT_BANKMAP, BankMapEnum::parse(argv.front()));
}

template <> void
Controller::exec <Token::memory, Token::set, Token::unmappingtype> (Arguments& argv, long param)
{
    amiga.configure(OPT_UNMAPPING_TYPE, UnmappedMemoryEnum::parse(argv.front()));
}

template <> void
Controller::exec <Token::memory, Token::set, Token::raminitpattern> (Arguments& argv, long param)
{
    amiga.configure(OPT_RAM_INIT_PATTERN, RamInitPatternEnum::parse(argv.front()));
}

template <> void
Controller::exec <Token::memory, Token::inspect, Token::state> (Arguments& argv, long param)
{
    dump(amiga.mem, Dump::State);
}

template <> void
Controller::exec <Token::memory, Token::inspect, Token::bankmap> (Arguments& argv, long param)
{
    dump(amiga.mem, Dump::BankMap);
}

template <> void
Controller::exec <Token::memory, Token::inspect, Token::checksums> (Arguments& argv, long param)
{
    dump(amiga.mem, Dump::Checksums);
}


//
// CPU
//

template <> void
Controller::exec <Token::cpu, Token::inspect, Token::state> (Arguments& argv, long param)
{
    dump(amiga.cpu, Dump::State);
}

template <> void
Controller::exec <Token::cpu, Token::inspect, Token::registers> (Arguments& argv, long param)
{
    dump(amiga.cpu, Dump::Registers);
}


//
// CIA
//

template <> void
Controller::exec <Token::cia, Token::config> (Arguments &argv, long param)
{
    if (param == 0) {
        dump(amiga.ciaA, Dump::Config);
    } else {
        dump(amiga.ciaB, Dump::Config);
    }
}

template <> void
Controller::exec <Token::cia, Token::set, Token::revision> (Arguments &argv, long param)
{
    if (param == 0) {
        amiga.ciaA.configure(OPT_CIA_REVISION, CIARevisionEnum::parse(argv.front()));
    } else {
        amiga.ciaB.configure(OPT_CIA_REVISION, CIARevisionEnum::parse(argv.front()));
    }
}

template <> void
Controller::exec <Token::cia, Token::set, Token::todbug> (Arguments &argv, long param)
{
    if (param == 0) {
        amiga.ciaA.configure(OPT_TODBUG, parseBool(argv.front()));
    } else {
        amiga.ciaB.configure(OPT_TODBUG,parseBool(argv.front()));
    }
}

template <> void
Controller::exec <Token::cia, Token::set, Token::esync> (Arguments &argv, long param)
{
    if (param == 0) {
        amiga.ciaA.configure(OPT_ECLOCK_SYNCING, parseBool(argv.front()));
    } else {
        amiga.ciaB.configure(OPT_ECLOCK_SYNCING, parseBool(argv.front()));
    }
}

template <> void
Controller::exec <Token::cia, Token::inspect, Token::state> (Arguments& argv, long param)
{
    if (param == 0) {
        dump(amiga.ciaA, Dump::State);
    } else {
        dump(amiga.ciaB, Dump::State);
    }
}

template <> void
Controller::exec <Token::cia, Token::inspect, Token::registers> (Arguments& argv, long param)
{
    if (param == 0) {
        dump(amiga.ciaA, Dump::Registers);
    } else {
        dump(amiga.ciaB, Dump::Registers);
    }
}

template <> void
Controller::exec <Token::cia, Token::inspect, Token::tod> (Arguments& argv, long param)
{
    if (param == 0) {
        dump(amiga.ciaA.tod, Dump::State);
    } else {
        dump(amiga.ciaB.tod, Dump::State);
    }
}


//
// Agnus
//

template <> void
Controller::exec <Token::agnus, Token::config> (Arguments &argv, long param)
{
    dump(amiga.agnus, Dump::Config);
}

template <> void
Controller::exec <Token::agnus, Token::set, Token::revision> (Arguments &argv, long param)
{
    amiga.configure(OPT_AGNUS_REVISION, AgnusRevisionEnum::parse(argv.front()));
}

template <> void
Controller::exec <Token::agnus, Token::set, Token::slowrammirror> (Arguments &argv, long param)
{
    amiga.configure(OPT_SLOW_RAM_MIRROR, parseBool(argv.front()));
}

template <> void
Controller::exec <Token::agnus, Token::inspect, Token::state> (Arguments &argv, long param)
{
    dump(amiga.agnus, Dump::State);
}

template <> void
Controller::exec <Token::agnus, Token::inspect, Token::registers> (Arguments &argv, long param)
{
    dump(amiga.agnus, Dump::Registers);
}

template <> void
Controller::exec <Token::agnus, Token::inspect, Token::events> (Arguments &argv, long param)
{
    dump(amiga.agnus, Dump::Events);
}


//
// Blitter
//

template <> void
Controller::exec <Token::blitter, Token::config> (Arguments& argv, long param)
{
    dump(amiga.agnus.blitter, Dump::Config);
}

template <> void
Controller::exec <Token::blitter, Token::set, Token::accuracy> (Arguments &argv, long param)
{
    amiga.configure(OPT_BLITTER_ACCURACY, parseDec(argv.front()));
}

template <> void
Controller::exec <Token::blitter, Token::inspect, Token::state> (Arguments& argv, long param)
{
    dump(amiga.agnus.blitter, Dump::State);
}

template <> void
Controller::exec <Token::blitter, Token::inspect, Token::registers> (Arguments& argv, long param)
{
    dump(amiga.agnus.blitter, Dump::Registers);
}


//
// Copper
//

template <> void
Controller::exec <Token::copper, Token::inspect, Token::state> (Arguments& argv, long param)
{
    dump(amiga.agnus.copper, Dump::State);
}

template <> void
Controller::exec <Token::copper, Token::inspect, Token::registers> (Arguments& argv, long param)
{
    dump(amiga.agnus.copper, Dump::Registers);
}


//
// Denise
//

template <> void
Controller::exec <Token::denise, Token::config> (Arguments& argv, long param)
{
    dump(amiga.denise, Dump::Config);
}

template <> void
Controller::exec <Token::denise, Token::set, Token::revision> (Arguments &argv, long param)
{
    amiga.configure(OPT_DENISE_REVISION, DeniseRevisionEnum::parse(argv.front()));
}

/*
template <> void
Controller::exec <Token::denise, Token::set, Token::palette> (Arguments &argv, long param)
{
    amiga.configure(OPT_PALETTE, PaletteEnum::parse(argv.front()));
}
*/

template <> void
Controller::exec <Token::denise, Token::set, Token::clxsprspr> (Arguments &argv, long param)
{
    amiga.configure(OPT_CLX_SPR_SPR, parseBool(argv.front()));
}

template <> void
Controller::exec <Token::denise, Token::set, Token::clxsprplf> (Arguments &argv, long param)
{
    amiga.configure(OPT_CLX_SPR_PLF, parseBool(argv.front()));
}

template <> void
Controller::exec <Token::denise, Token::set, Token::clxplfplf> (Arguments &argv, long param)
{
    amiga.configure(OPT_CLX_PLF_PLF, parseBool(argv.front()));
}

template <> void
Controller::exec <Token::denise, Token::set, Token::borderblank> (Arguments &argv, long param)
{
    amiga.configure(OPT_BRDRBLNK, parseBool(argv.front()));
}

template <> void
Controller::exec <Token::denise, Token::inspect, Token::state> (Arguments& argv, long param)
{
    dump(amiga.denise, Dump::State);
}

template <> void
Controller::exec <Token::denise, Token::inspect, Token::registers> (Arguments& argv, long param)
{
    dump(amiga.denise, Dump::Registers);
}


//
// Audio
//

template <> void
Controller::exec <Token::audio, Token::config> (Arguments& argv, long param)
{
    dump(amiga.paula.muxer, Dump::Config);
}

template <> void
Controller::exec <Token::audio, Token::set, Token::sampling> (Arguments& argv, long param)
{
    amiga.configure(OPT_SAMPLING_METHOD, SamplingMethodEnum::parse(argv.front()));
}

template <> void
Controller::exec <Token::audio, Token::set, Token::filter> (Arguments& argv, long param)
{
    amiga.configure(OPT_FILTER_TYPE, FilterTypeEnum::parse(argv.front()));
}

template <> void
Controller::exec <Token::audio, Token::set, Token::volume> (Arguments& argv, long param)
{
    switch (param) {
            
        case 0: amiga.configure(OPT_AUDVOL, 0, parseDec(argv.front())); break;
        case 1: amiga.configure(OPT_AUDVOL, 1, parseDec(argv.front())); break;
        case 2: amiga.configure(OPT_AUDVOL, 2, parseDec(argv.front())); break;
        case 3: amiga.configure(OPT_AUDVOL, 3, parseDec(argv.front())); break;
        case 4: amiga.configure(OPT_AUDVOLL, parseDec(argv.front())); break;
        case 5: amiga.configure(OPT_AUDVOLR, parseDec(argv.front())); break;
            
        default:
            assert(false);
    }
}

template <> void
Controller::exec <Token::audio, Token::set, Token::pan> (Arguments& argv, long param)
{
    amiga.configure(OPT_AUDPAN, param, parseDec(argv.front()));
}

template <> void
Controller::exec <Token::audio, Token::inspect, Token::state> (Arguments& argv, long param)
{
    dump(amiga.paula.muxer, Dump::State);
}

template <> void
Controller::exec <Token::audio, Token::inspect, Token::registers> (Arguments& argv, long param)
{
    dump(amiga.paula.muxer, Dump::Registers);
}


//
// Paula
//

template <> void
Controller::exec <Token::paula, Token::inspect, Token::state> (Arguments& argv, long param)
{
    dump(amiga.paula, Dump::State);
}

template <> void
Controller::exec <Token::paula, Token::inspect, Token::registers> (Arguments& argv, long param)
{
    dump(amiga.paula, Dump::Registers);
}


//
// RTC
//

template <> void
Controller::exec <Token::rtc, Token::config> (Arguments& argv, long param)
{
    dump(amiga.rtc, Dump::Config);
}

template <> void
Controller::exec <Token::rtc, Token::inspect, Token::registers> (Arguments& argv, long param)
{
    dump(amiga.rtc, Dump::Registers);
}

template <> void
Controller::exec <Token::rtc, Token::set, Token::revision> (Arguments &argv, long param)
{
    amiga.configure(OPT_RTC_MODEL, RTCRevisionEnum::parse(argv.front()));
}


//
// Keyboard
//

template <> void
Controller::exec <Token::keyboard, Token::config> (Arguments& argv, long param)
{
    dump(amiga.keyboard, Dump::Config);
}

template <> void
Controller::exec <Token::keyboard, Token::set, Token::accuracy> (Arguments &argv, long param)
{
    amiga.configure(OPT_ACCURATE_KEYBOARD, parseBool(argv.front()));
}

template <> void
Controller::exec <Token::keyboard, Token::inspect> (Arguments& argv, long param)
{
    dump(amiga.keyboard, Dump::State);
}


//
// Mouse
//

template <> void
Controller::exec <Token::mouse, Token::config> (Arguments& argv, long param)
{
    dump(amiga.keyboard, Dump::Config);
}

template <> void
Controller::exec <Token::mouse, Token::set, Token::velocity> (Arguments &argv, long param)
{
    amiga.configure(OPT_MOUSE_VELOCITY, PORT_1, parseDec(argv.front()));
    amiga.configure(OPT_MOUSE_VELOCITY, PORT_2, parseDec(argv.front()));
}

template <> void
Controller::exec <Token::mouse, Token::set, Token::pullup> (Arguments &argv, long param)
{
    amiga.configure(OPT_PULLUP_RESISTORS, parseBool(argv.front()));
}

template <> void
Controller::exec <Token::mouse, Token::inspect> (Arguments& argv, long param)
{
    dump(amiga.keyboard, Dump::State);
}


//
// Serial port
//

template <> void
Controller::exec <Token::serial, Token::config> (Arguments& argv, long param)
{
    dump(amiga.serialPort, Dump::Config);
}

template <> void
Controller::exec <Token::serial, Token::set, Token::device> (Arguments &argv, long param)
{
    amiga.configure(OPT_SERIAL_DEVICE, SerialPortDeviceEnum::parse(argv.front()));
}

template <> void
Controller::exec <Token::serial, Token::inspect> (Arguments& argv, long param)
{
    dump(amiga.serialPort, Dump::State);
}


//
// Disk controller
//

template <> void
Controller::exec <Token::dc, Token::config> (Arguments& argv, long param)
{
    dump(amiga.paula.diskController, Dump::Config);
}

template <> void
Controller::exec <Token::dc, Token::inspect> (Arguments& argv, long param)
{
    dump(amiga.paula.diskController, Dump::Registers);
}

template <> void
Controller::exec <Token::dc, Token::speed> (Arguments& argv, long param)
{
    amiga.configure(OPT_DRIVE_SPEED, parseDec(argv.front()));
}

template <> void
Controller::exec <Token::dc, Token::dsksync, Token::autosync> (Arguments& argv, long param)
{
    amiga.configure(OPT_AUTO_DSKSYNC, parseBool(argv.front()));
}

template <> void
Controller::exec <Token::dc, Token::dsksync, Token::lock> (Arguments& argv, long param)
{
    amiga.configure(OPT_LOCK_DSKSYNC, parseBool(argv.front()));
}


//
// Df0, Df1, Df2, Df3
//

template <> void
Controller::exec <Token::dfn, Token::config> (Arguments& argv, long param)
{
    dump(*amiga.df[param], Dump::Config);
}

template <> void
Controller::exec <Token::dfn, Token::audiate, Token::insert> (Arguments& argv, long param)
{
    amiga.configure(OPT_DRIVE_INSERT_NOISE, param, parseBool(argv.front()));
}

template <> void
Controller::exec <Token::dfn, Token::audiate, Token::eject> (Arguments& argv, long param)
{
    amiga.configure(OPT_DRIVE_EJECT_NOISE, param, parseBool(argv.front()));
}

template <> void
Controller::exec <Token::dfn, Token::audiate, Token::step> (Arguments& argv, long param)
{
    amiga.configure(OPT_DRIVE_STEP_NOISE, param, parseBool(argv.front()));
}

template <> void
Controller::exec <Token::dfn, Token::audiate, Token::poll> (Arguments& argv, long param)
{
    amiga.configure(OPT_DRIVE_POLL_NOISE, param, parseBool(argv.front()));
}

template <> void
Controller::exec <Token::dfn, Token::eject> (Arguments& argv, long param)
{
    amiga.df[param]->ejectDisk();
}

template <> void
Controller::exec <Token::dfn, Token::connect> (Arguments& argv, long param)
{
    amiga.configure(OPT_DRIVE_CONNECT, param, true);
}

template <> void
Controller::exec <Token::dfn, Token::disconnect> (Arguments& argv, long param)
{
    amiga.configure(OPT_DRIVE_CONNECT, param, false);
}

template <> void
Controller::exec <Token::dfn, Token::insert> (Arguments& argv, long param)
{
    string path = argv.front();
    
    try {
        ADFFile *adf = AmigaFile::make <ADFFile> (path.c_str());
        Disk *disk = Disk::makeWithFile(adf);
        amiga.df[param]->insertDisk(disk);
        
    } catch (VAError &err) {
        console << "Failed to insert disk: " << err.what() << '\n';
    }
}

template <> void
Controller::exec <Token::dfn, Token::set, Token::model> (Arguments& argv, long param)
{
    amiga.configure(OPT_DRIVE_TYPE, param, DriveTypeEnum::parse(argv.front()));
}

template <> void
Controller::exec <Token::dfn, Token::set, Token::mechanics> (Arguments& argv, long param)
{
    amiga.configure(OPT_EMULATE_MECHANICS, param, parseBool(argv.front()));
}

template <> void
Controller::exec <Token::dfn, Token::inspect> (Arguments& argv, long param)
{
    dump(*amiga.df[param], Dump::State);
}
