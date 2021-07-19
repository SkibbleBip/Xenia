/**************************************************************************
Copyright (C) 2021 SkibbleBip

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

**************************************************************************/
/***************************************************************************
* File:  Speech.c
* Author:  SkibbleBip
* Procedures:
* speak_init    -Hack for initializing the speaking engine, as the default
*                       FLITE API is broken
* speak_destroy -Frees the memory allocated by the initialized speaking engine
* speak         -Speaks the inputted text
***************************************************************************/

#include "Speech.h"
#include "cst_diphone.h"
#include "./flite/lang/usenglish/usenglish.h"
#include "./flite/lang/cmulex/cmu_lex.h"
/*Oh my god I want to punch the guy in the face who maintained the
*flite api post-CMU
*/

extern cst_diphone_db cmu_us_kal16_db;
/*such a terrible implementation in flite, but in order to initialize the TTS
* engline I need to declare this externally
*/


/***************************************************************************
* cst_voice* speak_init(void)
* Author: SkibbleBip
* Date: 07/17/2021
* Description: Hack for generating and initializing the voice engine for
*       speaking. FLITE's function is broken and I had to create it myself
*
* Parameters:
*        speak_init     O/P     cst_voice*      Returned pointer to the voice
*                                                       engine
**************************************************************************/
cst_voice* speak_init(void)
{
        /*This following section is entirely ripped from CMU's cmu_us_kal16.c
        *file. The reason why is because the kal16 diphone function included
        *in the API is absolutly wrong and dereferences addresses that no longer
        *exist. meaning, the cst_voice* returned by
        *cst_voice *register_cmu_us_kal16(const char *voxdir) is an invalid
        *pointer and will segfault. this is done by returning an INVALID
        *GLOBAL POINTER in a .C file. Who the heck wrote that
        *stuff? Better yet, how did this get past quality control? I can see
        *they did that to try to check if the cst_voice* was declared or not,
        *but there are better ways to do that. Don't friggen make global
        *pointers in .c files.
        */

        cst_voice *v;
        cst_lexicon *lex;
        v = new_voice();
        v->name = "kal16";
        usenglish_init(v);

        feat_set_string(v->features,"name","cmu_us_kal16");

        feat_set_float(v->features,"int_f0_target_mean",95.0);
        feat_set_float(v->features,"int_f0_target_stddev",11.0);
            feat_set_float(v->features,"duration_stretch",1.1);

        /* Lexicon */
        lex = cmu_lex_init();
        feat_set(v->features,"lexicon",lexicon_val(lex));
        feat_set(v->features,"postlex_func",uttfunc_val(lex->postlex));

        /* Waveform synthesis */
        feat_set(v->features,"wave_synth_func",uttfunc_val(&diphone_synth));
        feat_set(v->features,"diphone_db",diphone_db_val(&cmu_us_kal16_db));
        feat_set_int(v->features,"sample_rate",cmu_us_kal16_db.sts->sample_rate);
        /*    feat_set_string(v->features,"join_type","simple_join"); */
        feat_set_string(v->features,"join_type","modified_lpc");
        feat_set_string(v->features,"resynth_type","fixed");
        //flite_init();

        return v;

}


/***************************************************************************
* void speak_destroy(cst_voice* v)
* Author: SKibbleBip
* Date: 07/18/2021
* Description: Destroys the allocated speaker engine and frees it
*
* Parameters:
*        v      I/O     cst_voice*      pointer to the voice engine
**************************************************************************/
void speak_destroy(cst_voice* v)
{
        delete_voice(v);

}

/***************************************************************************
* float speak(cst_voice* v, char* text)
* Author: SkibbleBip
* Date: 07/14/2021
* Description: function that takes a string in and speaks it out loud
*
* Parameters:
*        v      I/P     cst_voice*      pointer to the voice object
*        text   I/P     char*           The text to speak out loud
*        speak  O/P     float           The duration of speaking
**************************************************************************/
float speak(cst_voice* v, char* text)
{
        float seconds = flite_text_to_speech(text, v, "play");

        return seconds;
}
