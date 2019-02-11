/********************************************************copyrite.xic***/
/*                                                                     */
/*   Licensed Materials - Property of IBM                              */
/*                                                                     */
/*   This product contains "Restricted Materials of IBM":              */
/*      Program Number:   5798RXW                                      */
/*      Program Name:     IBM TCP/IP Version 1.2 for OS/2              */
/*   (C) Copyright IBM Corporation. 1990, 1991.                        */
/*                                                                     */
/*   All rights reserved.                                              */
/*                                                                     */
/*   US Government Users Restricted Rights -                           */
/*   Use, duplication or disclosure restricted by GSA ADP Schedule     */
/*   Contract with IBM Corp.                                           */
/*                                                                     */
/*   See IBM Copyright Instructions.                                   */
/*                                                                     */
/********************************************************copyrite.xic***/
/* RGUC.C */

/* user written */
/* client program */

#include <stdio.h>
#include <string.h>
#include <process.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include "rg.h"
#include <sys\socket.h>
#include <netdb.h>

int main(int argc, char **argv)
{
   int *result;
   char *message;
   CLIENT *cl;
   short sock = -1;
   struct sockaddr_in addr;
   struct hostent *hp;     /* Pointer to host info */
   if (argc<3) {
     printf("usage is %s hostname message\n",argv[0]);
     return 0;
   }

   message = argv[2];

   hp = gethostbyname(argv[1]);
   if (hp) {
           addr.sin_family = hp->h_addrtype;
           bcopy(hp->h_addr, (caddr_t)&(addr.sin_addr), hp->h_length);
           addr.sin_port = (unsigned short) 0;
   } else {
           printf("unknown host\n");
           return 1;
   }

   cl = clnttcp_create(&addr, MESSAGEPROG, MESSAGEVERS, &sock, 50, 500);
   if (cl==NULL) return 1;
   result = printmessage_1(&message,cl);
   printf("The result of the RPC call was %d\n",*result);
   return *result;
}
