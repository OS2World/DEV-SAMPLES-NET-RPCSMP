#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include <rpc/pmap_pro.h>
#include "rg.h"

// as we are passing a pointer to this function to svc_register, we must
// declare it as _Far16 _Cdecl, and the pointers passed must be _Seg16.
static void _Far16 _Cdecl messageprog_1(struct svc_req * _Seg16 rqstp, SVCXPRT * _Seg16 transp);

int main(int argc,char **argv) {
        SVCXPRT *transp;

        pmap_unset(MESSAGEPROG, MESSAGEVERS);

        transp = svcudp_create(RPC_ANYSOCK);
        if (transp == NULL) {
                fprintf(stderr, "cannot create udp service.\n");
                return 1;
        }
        if (!svc_register(transp, MESSAGEPROG, MESSAGEVERS, messageprog_1, IPPROTO_UDP)) {
                fprintf(stderr, "unable to register (MESSAGEPROG, MESSAGEVERS, udp).\n");
                return 1;
        }

        transp = svctcp_create(RPC_ANYSOCK, 0, 0);
        if (transp == NULL) {
                fprintf(stderr, "cannot create tcp service.\n");
                return 1;
        }
        if (!svc_register(transp, MESSAGEPROG, MESSAGEVERS, messageprog_1, IPPROTO_TCP)) {
                fprintf(stderr, "unable to register (MESSAGEPROG, MESSAGEVERS, tcp).\n");
                return 1;
        }
        svc_run();
        fprintf(stderr, "svc_run returned\n");
        return 1;
}

static void _Far16 _Cdecl messageprog_1(struct svc_req * _Seg16 rqstp, SVCXPRT * _Seg16 transp)
{
        union {
                char * _Seg16 printmessage_1_arg;
        } argument;
        char *pch32;
        char *result;
        xdrproc_t xdr_argument,xdr_result;
        int * (* local)(char **);

        switch (rqstp->rq_proc) {
        case NULLPROC:
                svc_sendreply(transp, xdr_void, NULL);
                return;

        case PRINTMESSAGE:
                xdr_argument = xdr_wrapstring;
                xdr_result = xdr_int;
//                local = (char *(*)()) printmessage_1;
                local = printmessage_2;
                break;

        default:
                svcerr_noproc(transp);
                return;
        }
        bzero(&argument, sizeof(argument));
        if (!svc_getargs(transp, xdr_argument, &argument)) {
                svcerr_decode(transp);
                return;
        }
//        result = (*local)(&argument, rqstp);
// Note! argument.printmessage_1_arg is a 16 bit pointer.  We must pass
// a 32 bit pointer to a 32 bit pointer when we call local (printmessage_1)
// so it will not do to pass (char **)&argument -- we would be passing a
// 32 bit pointer to a 16 bit pointer.
        pch32=argument.printmessage_1_arg;
        result =(char *)((*local)((char **)&pch32));
        if (result != NULL && !svc_sendreply(transp, xdr_result, result)) {
                svcerr_systemerr(transp);
        }
        if (!svc_freeargs(transp, xdr_argument, &argument)) {
                fprintf(stderr, "unable to free arguments\n");
                exit(1);
        }
}

