#include <stdio.h>
#include <stdlib.h>

#include "vrt.h"
#include "cache/cache.h"

#include "vcc_if.h"

char *cow;

int
init_function(struct vmod_priv *priv, const struct VCL_conf *conf)
{
	/* init global state valid for the whole VCL life */
	cow = "< init >\n\t\t\t\t\t\t\t  \\\n\t\t\t\t\t\t\t   \\ ^__^\n"
	    "\t\t\t\t\t\t\t     (oo)\\_______\n\t\t\t\t\t\t\t     (__)\\"
	    "       )\\/\\\n\t\t\t\t\t\t\t         ||----w |\n\t\t\t\t\t\t\t"
	    "         ||     ||\n";
	return (0);
}

VCL_STRING
vmod_cowsay_synth(VRT_CTX)
{
        unsigned  u;
        struct vsb *vsb;

        u = WS_Reserve(ctx->ws, 0);
        vsb = VSB_new(NULL, ctx->ws->f, u, VSB_AUTOEXTEND);
	VSB_cat(vsb, "< body >\n");
	VSB_cat(vsb, cow);
        VSB_finish(vsb);
        WS_Release(ctx->ws, VSB_len(vsb) + 1);
        return (vsb->s_buf);
}
