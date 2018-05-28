/* 
 * File: SSTT.cpp
 * Author: Dagim Sisay <dagiopia@gmail.com>
 * License: AGPL
 * Date: May, 2018
*/

#include "sense/audio/SSTT.hpp"

SSTT::SSTT()
{
    cfg = cmd_ln_init(NULL, ps_args(), TRUE,
                    "-hmm", @SPHINX_MODEL_DIR@ "/en-us/en-us",
		    "-lm", @SPHINX_MODEL_DIR@ "/en-us/en-us.lm.bin",
		    "-dict", @SPHINX_MODEL_DIR@ "/en-us/cmudict-en-us.dict",
		    "-reduce-noise", "yes",
		    NULL);
    if( cfg == NULL) { fprintf(stderr, "Error Creating Config Object\n"); exit(1); }

    ps = ps_init(cfg);
    if (ps == NULL) { fprintf(stderr, "Error Initializing Recognizer\n"); exit(1); }
    ad = ad_open_dev(NULL, SAMPLE_RATE);
    if (ad == NULL) { fprintf(stderr, "Error Opening Device\n"; exit(1); }
    if (ad_start_rec(ad) < 0) { fprintf(stderr, "Error Starting Recording\n"); exit(1); }
    if (ps_start_utt(ps) < 0) { fprintf(stderr, "Error Starting Utterance\n"); exit(1); }
    
    utt_started = FALSE;
    printf("Ready...\n");

    while (1) {
        if ((k = ad_read(ad, adbuf, 2048)) < 0)
	    fprintf(stderr, "Errpr Reading Audio Data\n");
	ps_process_raw(ps, adbuf, k, FALSE, FALSE);
	in_speech = ps_get_in_speech(ps);
	if (in_speech && !utt_started) { 
	    utt_started = TRUE;
	    printf("Listening...\n");
	}
	if (!in_speech && utt_started) {
	    ps_end_utt(ps);
	    hyp = ps_get_hyp(ps, NULL);
	    if (hyp != NULL)
	        printf("Text: %s\n", hyp);

	    if (ps_start_utt(ps) < 0)
	        fprintf(stderr, "Error Starting Utterance\n");
	    utt_started = FALSE;
	    printf("Ready...\n");
       }
   }
}
