#include "nng/supplemental/nanolib/conf.h"
#include "nng/supplemental/nanolib/nanolib.h"

#include "nuts.h"

#if defined(ENABLE_NANOMQ_TESTS)
#define OLD_CONF_PATH                                            \
	"../../../../../nng/src/supplemental/nanolib/test_conf/" \
	"nmq_old_test.conf"
#define CONF_PATH \
	"../../../../../nng/src/supplemental/nanolib/test_conf/nmq_test.conf"
#define PKCS11_OLD_CONF_PATH                                             \
	"../../../../../nng/src/supplemental/nanolib/test_conf/"          \
	"nmq_tls_pkcs11_old_test.conf"
#define PKCS11_CONF_PATH \
	"../../../../../nng/src/supplemental/nanolib/test_conf/nmq_tls_pkcs11_test.conf"
#else
#define OLD_CONF_PATH \
	"../../../../src/supplemental/nanolib/test_conf/nmq_old_test.conf"
#define CONF_PATH \
	"../../../../src/supplemental/nanolib/test_conf/nmq_test.conf"
#define PKCS11_OLD_CONF_PATH \
	"../../../../src/supplemental/nanolib/test_conf/nmq_tls_pkcs11_old_test.conf"
#define PKCS11_CONF_PATH \
	"../../../../src/supplemental/nanolib/test_conf/nmq_tls_pkcs11_test.conf"
#endif


conf *
get_test_conf(const char *conf_path)
{
	conf *nmq_conf = nng_zalloc(sizeof(conf));
	if(nmq_conf == NULL) {
		return NULL;
	}
	conf_init(nmq_conf);
	nmq_conf->conf_file = nng_strdup(conf_path);

	return nmq_conf;
}

void
test_get_size(void)
{
	char    *str_size1   = "3KB";
	char    *str_size2   = "2MB";
	char    *str_size3   = "1GB";
	char    *str_size_tb = "1TB";
	char    *str_size_0  = "0s";
	uint64_t size        = 0;

	NUTS_PASS(get_size(str_size1, &size));
	NUTS_TRUE(size == 3 * 1024);
	NUTS_PASS(get_size(str_size2, &size));
	NUTS_TRUE(size == 2 * 1024 * 1024);
	NUTS_PASS(get_size(str_size3, &size));
	NUTS_TRUE(size == 1 * 1024 * 1024 * 1024);

	NUTS_FAIL(get_size(str_size_tb, &size), -1);
	NUTS_FAIL(get_size(str_size_0, &size), -1);
}

void
test_get_time(void)
{
	char    *str_time_s = "12s";
	char    *str_time_m = "24m";
	char    *str_time_h = "6h";
	char    *str_dflt   = "12dflt";
	char    *str_fail   = "fail";
	uint64_t second     = 0;

	NUTS_PASS(get_time(str_time_s, &second));
	NUTS_TRUE(second == 12);
	NUTS_PASS(get_time(str_time_m, &second));
	NUTS_TRUE(second == 24 * 60);
	NUTS_PASS(get_time(str_time_h, &second));
	NUTS_TRUE(second == 6 * 3600);

	// TODO: Should this be considered as fail?
	NUTS_PASS(get_time(str_dflt, &second));

	NUTS_FAIL(get_time(str_fail, &second), -1);
}

void
test_conf_parse(void)
{
#ifndef NNG_PLATFORM_WINDOWS
	conf *conf = get_test_conf(OLD_CONF_PATH);
	NUTS_TRUE(conf != NULL);
	conf_parse(conf);
	NUTS_TRUE(strncmp(conf->url, "nmq-tcp://0.0.0.0:1883", 22) == 0);

	print_conf(conf);

	conf_fini(conf);
#endif
}

void
test_conf_parse_ver2(void)
{
	conf *conf = get_test_conf(CONF_PATH);
	NUTS_TRUE(conf != NULL);
	conf_parse_ver2(conf);
	NUTS_TRUE(strncmp(conf->url, "nmq-tcp://0.0.0.0:1883", 22) == 0);

	print_conf(conf);

	conf_fini(conf);
}

void
test_conf_parse_tls_pkcs11_old(void)
{
	conf_tls tls;
	conf_tls_init(&tls);
	conf_tls_parse(&tls, PKCS11_OLD_CONF_PATH, "\0", "\0");

	NUTS_TRUE(NULL != tls.keyfile);
	NUTS_TRUE(NULL != tls.key);
	NUTS_TRUE(
	    0 == strcmp(tls.keyfile, "pkcs11:token=NanoMQ;object=broker-key;type=private"));
	NUTS_TRUE(0 == strcmp(tls.key, tls.keyfile));

	NUTS_TRUE(NULL != tls.certfile);
	NUTS_TRUE(NULL != tls.cert);
	NUTS_TRUE(
	    0 == strcmp(tls.certfile, "pkcs11:token=NanoMQ;object=broker-cert;type=cert"));
	NUTS_TRUE(0 == strcmp(tls.cert, tls.certfile));
	NUTS_TRUE(NULL != tls.cafile);
	NUTS_TRUE(NULL != tls.ca);
	NUTS_TRUE(
	    0 == strcmp(tls.cafile, "pkcs11:token=NanoMQ;object=broker-ca;type=cert"));
	NUTS_TRUE(0 == strcmp(tls.ca, tls.cafile));

	conf_tls_destroy(&tls);
}

void
test_conf_parse_ver2_tls_pkcs11(void)
{
	conf *conf = get_test_conf(PKCS11_CONF_PATH);
	NUTS_TRUE(conf != NULL);
	conf_parse_ver2(conf);

	NUTS_TRUE(NULL != conf->tls.keyfile);
	NUTS_TRUE(NULL != conf->tls.key);
	NUTS_TRUE(
	    0 == strcmp(conf->tls.keyfile, "pkcs11:token=NanoMQ;object=broker-key;type=private"));
	NUTS_TRUE(0 == strcmp(conf->tls.key, conf->tls.keyfile));

	NUTS_TRUE(NULL != conf->tls.certfile);
	NUTS_TRUE(NULL != conf->tls.cert);
	NUTS_TRUE(
	    0 == strcmp(conf->tls.certfile, "pkcs11:token=NanoMQ;object=broker-cert;type=cert"));
	NUTS_TRUE(0 == strcmp(conf->tls.cert, conf->tls.certfile));
	NUTS_TRUE(NULL != conf->tls.cafile);
	NUTS_TRUE(NULL != conf->tls.ca);
	NUTS_TRUE(
	    0 == strcmp(conf->tls.cafile, "pkcs11:token=NanoMQ;object=broker-ca;type=cert"));
	NUTS_TRUE(0 == strcmp(conf->tls.ca, conf->tls.cafile));

	conf_fini(conf);
}

NUTS_TESTS = {
   {"get size", test_get_size},
   {"get time", test_get_time},
   {"conf parse v2", test_conf_parse_ver2},
   {"conf parse v2 pkcs11", test_conf_parse_ver2_tls_pkcs11},
   {"conf parse", test_conf_parse},
   {"conf parse pkcs11", test_conf_parse_tls_pkcs11_old},
   {NULL, NULL} 
};
