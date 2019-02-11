#include <rpc/rpc.h>
#include <time.h>
#include <string.h>
#include "rg.h"

static struct timeval TIMEOUT = { 25, 0 };

int *
printmessage_1(char **argp, CLIENT *clnt)
{
        static int res;
        char * _Seg16 pch16;

        pch16=*argp;
        bzero(&res, sizeof(res));
        if (clnt_call(clnt, PRINTMESSAGE, xdr_wrapstring, pch16,

// note! The 4th argument to the clnt_call macro is a _Seg16 pointer to
// a _Seg16 pointer.  It would not do to pass argp.  The compiler would
// convert argp to a _Seg16 pointer to an 0:32 pointer, which is the
// best it can do -- but this will trap in the 16 bit DLL.

          xdr_int, &res, TIMEOUT) != RPC_SUCCESS) {
                res=-99;
        }
        return (&res);
}

