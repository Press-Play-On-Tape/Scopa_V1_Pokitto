#include "Pokitto.h"
#include <LibAudio>
#include "src/Game.h"
#include "src/utils/GameCookie.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;

Game game;
GameCookie cookie;

Audio::Sink<4, PROJ_AUD_FREQ> audio;

int main(){
    

    // Initialise pokitto ..

    cookie.begin("SCOPA", sizeof(cookie), (char*)&cookie);

    PC::begin();
    PD::loadRGBPalette(palettePico);   
    PD::persistence = true;
    PD::setColor(5, 0);
    PD::setInvisibleColor(14);
    PC::setFrameRate(60);
    PD::setFont(fontC64);
    PD::adjustCharStep = 0;


    // Has the cookie been initialised?

    if (cookie.initialised != COOKIE_INITIALISED) {

        cookie.initialise();

    }

    game.setup(&cookie);
    
    while( PC::isRunning() ){

        if( !PC::update() )  continue;

        game.loop();

    }
    
    return 0;
}