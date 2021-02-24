// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"
#include "Exception.h"
#include "Amiga.h"

extern "C" {
void process(const void *ref, long id, long data)
{
    ((Controller *)ref)->processMessage(id, data);
}
}

Controller::Controller(Application &ref) : GUIComponent(ref)
{
}

Controller::~Controller()
{
    
}

void
Controller::init()
{
}

void
Controller::configure()
{
    /*
    // Register to the message queue
    amiga.queue.addListener(this, process);
    */
}

void
Controller::deinit()
{
    /*
    amiga.powerOff();
    amiga.queue.removeListener(this);
    */
}

void
Controller::processMessage(long id, long data)
{
    /*
    MsgType msg = (MsgType)id;

    switch (msg) {
            
        case MSG_POWER_ON:
            app.canvas.open();
            break;
            
        case MSG_POWER_OFF:
            app.canvas.close();
            break;

        case MSG_RUN:
            break;

        case MSG_PAUSE:
            break;

        case MSG_RESET:
            break;
                        
        case MSG_DISK_INSERT_NOISE:
            app.play(SoundID::insert);
            [[fallthrough]];
            
        case MSG_DISK_INSERT:
            break;

        case MSG_DISK_EJECT_NOISE:
            app.play(SoundID::eject);
            [[fallthrough]];
            
        case MSG_DISK_EJECT:
            break;

        case MSG_DRIVE_HEAD_NOISE:
            app.playClick();
            [[fallthrough]];
            
        case MSG_DRIVE_HEAD:
            return;

        case MSG_SHAKING:
            app.os.releaseMouse();
            break;
            
        default:
            return;
            
    }
    printf("%s\n", MsgTypeEnum::key(msg));
    */
}

bool
Controller::parseBool(string& token)
{
    /*
    if (token == "1" || token == "true" || token == "yes") return true;
    if (token == "0" || token == "false" || token == "no") return false;

    throw ParseBoolError("");
    */
    return 0;
}

long
Controller::parseNum(isize base, string& token)
{
    /*
    assert(base == 10 || base == 16);
    
    long result;
    
    std::stringstream ss;
    try { ss << (base == 10 ? std::dec : std::hex) << token; ss >> result; }
    catch (std::exception& err) { result = 1; }

    return result;
    */
    return 0;
}

void
Controller::dump(HardwareComponent &component, Dump::Category category)
{
    /*
    std::stringstream ss; string line;
    
    amiga.suspend();
    component.dump(category, ss);
    amiga.resume();
    
    while(std::getline(ss, line)) console << line << '\n';
    */
}
