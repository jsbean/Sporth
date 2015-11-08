#include <stdlib.h>
#include "plumber.h"

int sporth_mincer(sporth_stack *stack, void *ud)
{
    plumber_data *pd = ud;
    SPFLOAT out;
    sp_ftbl * ft;
    char *ftname;
    SPFLOAT time;
    SPFLOAT amp;
    SPFLOAT pitch;
    sp_mincer *mincer;

    switch(pd->mode) {
        case PLUMBER_CREATE:

#ifdef DEBUG_MODE
            fprintf(stderr, "mincer: Creating\n");
#endif

            sp_mincer_create(&mincer);
            plumber_add_module(pd, SPORTH_MINCER, sizeof(sp_mincer), mincer);
            break;
        case PLUMBER_INIT:

#ifdef DEBUG_MODE
            fprintf(stderr, "mincer: Initialising\n");
#endif

            if(sporth_check_args(stack, "fffs") != SPORTH_OK) {
                fprintf(stderr,"Not enough arguments for mincer\n");
                stack->error++;
                return PLUMBER_NOTOK;
            }
            ftname = sporth_stack_pop_string(stack);
            pitch = sporth_stack_pop_float(stack);
            amp = sporth_stack_pop_float(stack);
            time = sporth_stack_pop_float(stack);
            mincer = pd->last->ud;
            
            if(plumber_ftmap_search(pd, ftname, &ft) == PLUMBER_NOTOK) {
                stack->error++;
                return PLUMBER_NOTOK;
            }

            sp_mincer_init(pd->sp, mincer, ft);
            sporth_stack_push_float(stack, 0);
            break;
        case PLUMBER_COMPUTE:
            pitch = sporth_stack_pop_float(stack);
            amp = sporth_stack_pop_float(stack);
            time = sporth_stack_pop_float(stack);
            mincer = pd->last->ud;
            mincer->time = time;
            mincer->amp = amp;
            mincer->pitch = pitch;
            sp_mincer_compute(pd->sp, mincer, NULL, &out);
            sporth_stack_push_float(stack, out);
            free(ftname);
            break;
        case PLUMBER_DESTROY:
            mincer = pd->last->ud;
            sp_mincer_destroy(&mincer);
            break;
        default:
            fprintf(stderr, "mincer: Uknown mode!\n");
            break;
    }
    return PLUMBER_OK;
}
