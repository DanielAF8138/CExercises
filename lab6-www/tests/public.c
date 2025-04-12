#include <arpa/inet.h>
#include <assert.h>
#include <check.h>
#include <fcntl.h>
#include <inttypes.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../utils.h"
#include "tutils.h"

START_TEST (UNIT_build_address)
{
  printf ("======================================\n");
  printf ("PUBLIC\n");
  printf ("  UNIT build address\n");
  printf ("  Build a struct sockaddr_in for localhost, port 80\n");
  struct sockaddr_in localhost = build_address (0x7f000001, "80");
  ck_assert_int_eq (localhost.sin_port, htons (80));
  char *addr = inet_ntoa (localhost.sin_addr);
  ck_assert_str_eq (addr, "127.0.0.1");
}
END_TEST

START_TEST (UNIT_build_response)
{
  printf ("======================================\n");
  printf ("PUBLIC\n");
  printf ("  UNIT build response header\n");
  printf ("  Given a request for index.html, get the full\n");
  printf ("  response with the headers and the file contents. The hexdump\n");
  printf ("  of the response should be:\n");

  char *expected = "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html; charset=UTF-8\r\n"
    "Content-Length: 119\r\n"
    "\r\n"
    "<html>\n  <head>\n    <title>Success!</title>\n  </head>\n\n"
    "  <body>\n    <h2>It <i>really</i> works!</h2>\n  </body>\n</html>\n";
  hexdump (expected);

  char buffer[] = "GET /index.html HTTP/1.0\r\n\r\n";
  char *header = build_response (buffer);

  printf ("\n  Your response has the following hexdump:\n");
  hexdump (header);

  printf ("\n  Note that 0a is \\n and 0d is \\r\n");
  ck_assert_str_eq (header, expected);
  free (header);
}
END_TEST

START_TEST (UNIT_build_response_non_exist)
{
  printf ("======================================\n");
  printf ("PUBLIC\n");
  printf ("  UNIT build response non exist\n");
  printf ("  Given a GET request for yadda.txt, get the URI\n");

  char buffer[] = "GET /yadda.txt HTTP/1.0\r\n\r\n";
  char *uri = build_response (buffer);
  ck_assert (uri == NULL);
}
END_TEST

START_TEST (UNIT_build_response_bad)
{
  printf ("======================================\n");
  printf ("PUBLIC\n");
  printf ("  UNIT build response bad\n");
  printf ("  Given a bad request (\"GETyadda.txt\"), should get NULL\n");

  char buffer[] = "GETyadda.txt";
  char *uri = build_response (buffer);
  ck_assert (uri == NULL);
}
END_TEST


void public_tests (Suite *s)
{
  TCase *tc_public = tcase_create ("Public");
  tcase_add_test (tc_public, UNIT_build_address);
  tcase_add_test (tc_public, UNIT_build_response);
  tcase_add_test (tc_public, UNIT_build_response_non_exist);
  tcase_add_test (tc_public, UNIT_build_response_bad);
  suite_add_tcase (s, tc_public);
}

