/*
 * Copyright (c) 1997 - 2006 Kungliga Tekniska H�gskolan
 * (Royal Institute of Technology, Stockholm, Sweden). 
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 *
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *
 * 3. Neither the name of the Institute nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE. 
 */

#include "kadmin_locl.h"
#include "kadmin-commands.h"

RCSID("$Id$");

static int
do_mod_entry(krb5_principal principal, void *data)
{
    krb5_error_code ret;
    kadm5_principal_ent_rec princ;
    int mask = 0;
    struct modify_options *e = data;
    
    memset (&princ, 0, sizeof(princ));
    ret = kadm5_get_principal(kadm_handle, principal, &princ,
			      KADM5_PRINCIPAL | KADM5_ATTRIBUTES | 
			      KADM5_MAX_LIFE | KADM5_MAX_RLIFE |
			      KADM5_PRINC_EXPIRE_TIME |
			      KADM5_PW_EXPIRATION);
    if(ret) 
	return ret;

    if(e->max_ticket_life_string || 
       e->max_renewable_life_string ||
       e->expiration_time_string ||
       e->pw_expiration_time_string ||
       e->attributes_string ||
       e->kvno_integer != -1 ||
       e->constrained_delegation_string) {
	ret = set_entry(context, &princ, &mask, 
			e->max_ticket_life_string, 
			e->max_renewable_life_string, 
			e->expiration_time_string, 
			e->pw_expiration_time_string, 
			e->attributes_string);
	if(e->kvno_integer != -1) {
	    princ.kvno = e->kvno_integer;
	    mask |= KADM5_KVNO;
	}
	if (e->constrained_delegation_string) {
	    HDB_extension ext;
	    krb5_tl_data *tl, **ptl;
	    krb5_data buf;
	    krb5_principal p;
	    size_t size;
	    
	    memset(&ext, 0, sizeof(ext));
	    ext.mandatory = FALSE;
	    ext.data.element = choice_HDB_extension_data_allowed_to_delegate_to;
	    ext.data.u.allowed_to_delegate_to.len = 1;
	    ext.data.u.allowed_to_delegate_to.val = 
		emalloc(sizeof(ext.data.u.allowed_to_delegate_to.val[0]));
	    ret = krb5_parse_name(context, 
				  e->constrained_delegation_string,
				  &p);
	    ext.data.u.allowed_to_delegate_to.val[0] = *p;
	    free(p);
	    
	    ASN1_MALLOC_ENCODE(HDB_extension, buf.data, buf.length,
			       &ext, &size, ret);
	    free_HDB_extension(&ext);
	    if (ret)
		abort();
	    if (buf.length != size)
		abort();

	    tl = ecalloc(1, sizeof(*tl));
	    tl->tl_data_next = NULL;
	    tl->tl_data_type = KRB5_TL_EXTENSION;
	    tl->tl_data_length = buf.length;
	    tl->tl_data_contents = buf.data;

	    princ.n_tl_data++;
	    ptl = &princ.tl_data;
	    while (*ptl != NULL)
		ptl = &(*ptl)->tl_data_next;
	    *ptl = tl;

	    mask |= KADM5_TL_DATA;
	}
    } else
	ret = edit_entry(&princ, &mask, NULL, 0);
    if(ret == 0) {
	ret = kadm5_modify_principal(kadm_handle, &princ, mask);
	if(ret)
	    krb5_warn(context, ret, "kadm5_modify_principal");
    }
    
    kadm5_free_principal_ent(kadm_handle, &princ);
    return ret;
}

int
mod_entry(struct modify_options *opt, int argc, char **argv)
{
    krb5_error_code ret = 0;
    int i;

    for(i = 0; i < argc; i++) {
	ret = foreach_principal(argv[i], do_mod_entry, "mod", opt);
	if (ret)
	    break;
    }
    return ret != 0;
}

