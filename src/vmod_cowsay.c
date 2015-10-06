#include <stdio.h>
#include <stdlib.h>
#include "cowsay.h"
#include "vrt.h"
#include "cache/cache.h"

#include "vcc_if.h"
char *cow;

/* The init function is used to init variables or global state you want to be
   available for all the time the vcl is loaded. If you have stateless
   functions you don't have to care about this function
*/

int
init_function(struct vmod_priv *priv, const struct VCL_conf *conf)
{
	/* init global state valid for the whole VCL life */
	cow =
	    "\n ^__^\n"
	    " (oo)\\_______\n"
	    " (__)\\       )\\/\\\n"
	    "      ||----w |\n"
	    "      ||     ||\n";
	/* this 'cow' is now available for every other functions that will
	   be defined in this vmod */
	return (0);
}

VCL_STRING
vmod_cowsay_canonical(VRT_CTX)
{
	char *p;
	unsigned u, v;
	/* Reserve some work space, Varnish need to know the VMOD is
	   going to do something and it needs memory for it */
	u = WS_Reserve(ctx->ws, 0);
        /* We set a ponter to the front of the workspace area we have been
	   assigned by varnish */
	p = ctx->ws->f;
	/* now we can actually use the workspace we reserved and
	   we write the 'cow' on it*/
	v = snprintf(p, u, "%s", cow);
	v++;
	if (v > u) {
        /* If the reserved workspace was not enough for what we planned
	   to do, then we release it all and return NULL, this means
	   "Sorry, Varnish doesn't have enough space for what you want to
	   do, try to bump the workspace" */
		WS_Release(ctx->ws, 0);
		return (NULL);
	}
	/* If we reach this point it means everything went as expected
	   and we can update workspace with what we've used and give the rest
	   back to varnish */
	WS_Release(ctx->ws, v);
	/* This 'p' pointer is now ponting to the very beginning of the
	   workspace we reserved */
	return (p);
}

VCL_STRING
vmod_cowsay_vsb(VRT_CTX)
{
	/* This VMOD function shows you how to create response bodies
	   intended as synthetic objects. */

        unsigned  u;
        struct vsb *vsb;
        u = WS_Reserve(ctx->ws, 0);
	/* vsb.h is a very useful varnish library for manipulating strings,
	   use it instead of the canonical string libraries  */
        vsb = VSB_new(NULL, ctx->ws->f, u, VSB_AUTOEXTEND);
	VSB_printf(vsb, "** body **\n");
	VSB_cat(vsb, cow);
        VSB_finish(vsb);
        WS_Release(ctx->ws, VSB_len(vsb) + 1);
        return (vsb->s_buf);
}

/* This is more advanced and complete example based on the previous function */
VCL_STRING
vmod_cowsay_friends(VRT_CTX, VCL_STRING animal, VCL_STRING talk)
{
        unsigned  u;
        struct vsb *vsb;
        u = WS_Reserve(ctx->ws, 0);
        vsb = VSB_new(NULL, ctx->ws->f, u, VSB_AUTOEXTEND);
	if(!strcmp(animal, "cow")) {
		VSB_printf(vsb, "** %s **\n", talk);
		VSB_cat(vsb, cow);
	}

	if(!strcmp(animal, "bunny")) {
		VSB_printf(vsb, "** %s **\n", talk);
		VSB_cat(vsb, baby_bunny());
	}
        VSB_finish(vsb);
        WS_Release(ctx->ws, VSB_len(vsb) + 1);
        return (vsb->s_buf);
}
