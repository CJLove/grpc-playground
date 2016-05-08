/* ------------------------------------------------------------------------ *
 *  * file:        keytest.c                                                   *
 *   * purpose:     tests loading of a private key for certificate signing      *
 *    * author:      02/23/2004 Frank4DD                                         * 
 *     * ------------------------------------------------------------------------ */
#include <stdio.h>
#include <string.h>
#include <openssl/x509v3.h>
#include <openssl/objects.h>
#include <openssl/pem.h>
#include <openssl/evp.h>

int main(int argc, char **argv) {
   EVP_PKEY *privkey;
   FILE *fp;
   RSA *rsakey;

   /* ---------------------------------------------------------- *
 *     * Next function is essential to enable openssl functions     *
 *         ------------------------------------------------------------ */
   OpenSSL_add_all_algorithms();

   privkey = EVP_PKEY_new();

   fp = fopen (argv[1], "r");

   PEM_read_PrivateKey( fp, &privkey, NULL, NULL);

   fclose(fp);

   rsakey = EVP_PKEY_get1_RSA(privkey);

   if(RSA_check_key(rsakey)) {
     printf("RSA key is valid.\n");
   }
   else {
     printf("Error validating RSA key.\n");
   }

   RSA_print_fp(stdout, rsakey, 3);

   PEM_write_PrivateKey(stdout,privkey,NULL,NULL,0,0,NULL);

   exit(0);
}
