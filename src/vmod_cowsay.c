#include <stdio.h>
#include <stdlib.h>
#include "cowsay.h"
#include "vrt.h"
#include "cache/cache.h"

#include "vcc_if.h"
char *cow;

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
	VSL(SLT_Debug, 0, "Init_function: %s ", cow);
	return (0);
}

VCL_STRING
vmod_cowsay_header(VRT_CTX)
{
	char *p;
	unsigned u, v;

	u = WS_Reserve(ctx->ws, 0); /* Reserve some work space */
	p = ctx->ws->f;		/* Front of workspace area */
	v = snprintf(p, u, "%s", cow);
	v++;
	if (v > u) {
		/* No space, reset and leave */
		WS_Release(ctx->ws, 0);
		return (NULL);
	}
	/* Update work space with what we've used */
	WS_Release(ctx->ws, v);
	return (p);
}

VCL_STRING
vmod_cowsay_synth(VRT_CTX)
{
        unsigned  u;
        struct vsb *vsb;
        u = WS_Reserve(ctx->ws, 0);
        vsb = VSB_new(NULL, ctx->ws->f, u, VSB_AUTOEXTEND);
	VSB_printf(vsb, "** body **\n");
	VSB_cat(vsb, cow);
        VSB_finish(vsb);
        WS_Release(ctx->ws, VSB_len(vsb) + 1);
        return (vsb->s_buf);
}


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
