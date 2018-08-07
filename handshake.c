#include "openssl/bio.h"
#include "openssl/err.h"
#include "openssl/ssl.h"

#include "stdio.h"
#include "string.h"

int main() {
  /* Set up the library */
  SSL_library_init();
  SSL_load_error_strings();
  ERR_load_BIO_strings();
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();

  /* Set up the SSL context */
  SSL_CTX* ctx = SSL_CTX_new(TLSv1_2_method());

  /* Load the trust store */
  if (!SSL_CTX_load_verify_locations(ctx, "TrustStore.pem", NULL)) {
    fprintf(stderr, "Error loading trust store\n");
    ERR_print_errors_fp(stderr);
    SSL_CTX_free(ctx);
    return -1;
  }

  /* Setup the connection */
  BIO* bio = BIO_new_ssl_connect(ctx);

  /* Set the SSL_MODE_AUTO_RETRY flag */
  SSL* ssl;
  BIO_get_ssl(bio, &ssl);
  SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

  /* Create and setup the connection */
  BIO_set_conn_hostname(bio, "embeddedassistant.googleapis.com:https");
  BIO_set_conn_port(bio, "443");

  if (BIO_do_handshake(bio) <= 0) {
    fprintf(stderr, "Error attempting to connect\n");
    ERR_print_errors_fp(stderr);
    BIO_free_all(bio);
    SSL_CTX_free(ctx);
    return -2;
  }

  /* Close the connection and free the context */
  BIO_free_all(bio);
  SSL_CTX_free(ctx);
  return 0;
}
