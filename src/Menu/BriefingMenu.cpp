/*
 *  This file is part of Dune Legacy.
 *
 *  Dune Legacy is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Dune Legacy is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Dune Legacy.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Menu/BriefingMenu.h>

#include <globals.h>

#include <FileClasses/GFXManager.h>
#include <FileClasses/TextManager.h>
#include <FileClasses/music/MusicPlayer.h>

#include <stdlib.h>

BriefingMenu::BriefingMenu(int newHouse,int mission,int type) : MentatMenu(newHouse) {
	this->mission = mission;
    this->type = type;

	Animation* anim = NULL;

	SDL_Surface* surf;
	SDL_Surface* surfPressed;

	surf = pGFXManager->getUIGraphic(UI_MentatProcced);
	surfPressed = pGFXManager->getUIGraphic(UI_MentatProcced_Pressed);
	proccedButton.setSurfaces(surf,false,surfPressed,false);
	proccedButton.setEnabled(false);
	proccedButton.setVisible(false);
	proccedButton.setOnClick(std::bind(&BriefingMenu::onProcced, this));
	windowWidget.addWidget(&proccedButton,Point(500,340),Point(surf->w,surf->h));

	surf = pGFXManager->getUIGraphic(UI_MentatRepeat);
	surfPressed = pGFXManager->getUIGraphic(UI_MentatRepeat_Pressed);
	repeatButton.setSurfaces(surf,false,surfPressed,false);
	repeatButton.setEnabled(false);
	repeatButton.setVisible(false);
	repeatButton.setOnClick(std::bind(&BriefingMenu::onRepeat, this));
	windowWidget.addWidget(&repeatButton,Point(350,340),Point(surf->w,surf->h));

	int missionnumber;
	if(mission != 22) {
		missionnumber = ((mission+1)/3)+1;
	} else {
		missionnumber = 9;
	}

	switch(type) {
		case DEBRIEFING_WIN: {
			anim = pGFXManager->getAnimation((rand() % 2 == 0) ? Anim_Win1 : Anim_Win2);
			text = pTextManager->getBriefingText(missionnumber,MISSION_WIN,house);
        } break;
		case DEBRIEFING_LOST: {
			anim = pGFXManager->getAnimation((rand() % 2 == 0) ? Anim_Lose1 : Anim_Lose2);
			text = pTextManager->getBriefingText(missionnumber,MISSION_LOSE,house);
        } break;
		default:
		case BRIEFING: {
		    anim = pGFXManager->getAnimation(getMissionSpecificAnim(missionnumber));
			text = pTextManager->getBriefingText(missionnumber,MISSION_DESCRIPTION,house);
		} break;
	}
	setText(text);
	animation.setAnimation(anim);
	windowWidget.addWidget(&animation,Point(256,96),Point(anim->getFrame()->w,anim->getFrame()->h));
}

BriefingMenu::~BriefingMenu() {
}

void BriefingMenu::onMentatTextFinished() {
	proccedButton.setEnabled(true);
	proccedButton.setVisible(true);
	repeatButton.setEnabled(true);
	repeatButton.setVisible(true);
}

int BriefingMenu::showMenu()
{
    switch(type) {
        case DEBRIEFING_WIN:
        {
            switch(house) {
                case HOUSE_HARKONNEN:
                case HOUSE_SARDAUKAR: {
                    musicPlayer->changeMusic(MUSIC_WIN_H);
                } break;

                case HOUSE_ATREIDES:
                case HOUSE_FREMEN: {
                    musicPlayer->changeMusic(MUSIC_WIN_A);
                } break;

                case HOUSE_ORDOS:
                case HOUSE_MERCENARY: {
                    musicPlayer->changeMusic(MUSIC_WIN_O);
                } break;
            }
        } break;

        case DEBRIEFING_LOST:
        {
            musicPlayer->changeMusic(MUSIC_LOSE);
        } break;

        case BRIEFING:
        {
            switch(house) {
                case HOUSE_HARKONNEN:
                case HOUSE_SARDAUKAR: {
                    musicPlayer->changeMusic(MUSIC_BRIEFING_H);
                } break;

                case HOUSE_ATREIDES:
                case HOUSE_FREMEN: {
                    musicPlayer->changeMusic(MUSIC_BRIEFING_A);
                } break;

                case HOUSE_ORDOS:
                case HOUSE_MERCENARY: {
                    musicPlayer->changeMusic(MUSIC_BRIEFING_O);
                } break;
            }
        } break;
    }

    return MentatMenu::showMenu();
}

void BriefingMenu::onRepeat() {
    setText(text);

	proccedButton.setEnabled(false);
	proccedButton.setVisible(false);
	repeatButton.setEnabled(false);
	repeatButton.setVisible(false);
}

void BriefingMenu::onProcced() {
	quit();
}
