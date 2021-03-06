/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: krakjoe <krakjoe@php.net>                                    |
  +----------------------------------------------------------------------+
*/

/* $Id$ */
#ifndef HAVE_HS_HOST
#define HAVE_HS_HOST

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "hs.h"

#include "../module.h"

zend_object_handlers HandlerSocket_Host_handlers;
zend_class_entry *HandlerSocket_Host_ce;

/* {{{ proto Host Host::__construct(string address) */
PHP_METHOD(Host, __construct) {
	zend_string *address = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &address) != SUCCESS) {
		return;
	}
	
	php_hs_host(address, getThis());
}

ZEND_BEGIN_ARG_INFO_EX(php_hs_host_construct_arginfo, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, address, IS_STRING, 0)
ZEND_END_ARG_INFO() /* }}} */

/* {{{ */
zend_function_entry php_hs_host_methods[] = {
	PHP_ME(Host, __construct, php_hs_host_construct_arginfo, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
static inline zend_object* php_hs_host_create(zend_class_entry *ce) {
	php_hs_host_t *h = (php_hs_host_t*) 
		ecalloc(1, sizeof(php_hs_host_t) + zend_object_properties_size(ce));

	zend_object_std_init(&h->std, ce);
	object_properties_init(&h->std, ce);	

	h->std.handlers = &HandlerSocket_Host_handlers;

	return &h->std;	
}

static inline void php_hs_host_free(zend_object *o) {
	php_hs_host_t *h = php_hs_host_fetch_from(o);

	if (h->address) {
		zend_string_release(h->address);
	}

	zend_object_std_dtor(o);
} /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(host) {
	zend_class_entry ce;

	memcpy(&HandlerSocket_Host_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	HandlerSocket_Host_handlers.offset = XtOffsetOf(php_hs_host_t, std);
	HandlerSocket_Host_handlers.free_obj = php_hs_host_free;

	INIT_NS_CLASS_ENTRY(ce, "HandlerSocket", "Host", php_hs_host_methods);
	HandlerSocket_Host_ce = zend_register_internal_class(&ce);
	HandlerSocket_Host_ce->create_object = php_hs_host_create;
} /* }}} */

/* {{{ */
char *php_hs_host_string(zval *host) {
	php_hs_host_t *h = 
		php_hs_host_fetch(host);
	return ZSTR_VAL(h->address);
} /* }}} */
#endif
