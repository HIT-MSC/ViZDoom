/*
 Copyright (C) 2016 by Wojciech Jaśkowski, Michał Kempka, Grzegorz Runc, Jakub Toczek, Marek Wydmuch

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/

#include "viz_shared_memory.h"
#include "viz_message_queue.h"
#include "viz_game.h"
#include "viz_input.h"
#include "viz_screen.h"

#include "doomstat.h"
#include "v_video.h"

bip::shared_memory_object vizSM;
size_t vizSMSize;
size_t vizSMGameStateAddress;
size_t vizSMInputStateAddress;
size_t vizSMScreenAddress;
char * vizSMName;

void VIZ_SMInit(const char * id){

	vizSMName = new char[strlen(VIZ_SM_NAME_BASE) + strlen(id) + 1];
	strcpy(vizSMName, VIZ_SM_NAME_BASE);
	strcat(vizSMName, id);

    try {
        bip::shared_memory_object::remove(vizSMName);
        vizSM = bip::shared_memory_object(bip::open_or_create, vizSMName, bip::read_write);

        size_t vizSMGameStateAddress = 0;
        size_t vizSMInputStateAddress = sizeof(VIZGameState);
        size_t vizSMScreenAddress = sizeof(VIZGameState) + sizeof(VIZInputState);

        vizSMSize = sizeof(VIZGameState) + sizeof(VIZInputState) + (sizeof(BYTE) * screen->GetWidth() * screen->GetHeight() * 4);
        vizSM.truncate(vizSMSize);
    }
    catch(bip::interprocess_exception &ex){
        printf("VIZ_SMInit: Error creating shared memory");
        VIZ_MQSend(VIZ_MSG_CODE_DOOM_ERROR);
        exit(1);
    }
}

void VIZ_SMClose(){
    //bip::shared_memory_object::remove(vizSMName);
	delete[] vizSMName;
}