#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "main/php_output.h"
#include "php_myext.h"

static int myext_output_handler(void **nothing, php_output_context *output_context)
{
	char *dup = NULL;

	dup = estrndup(output_context->in.data, output_context->in.used);
	php_strtoupper(dup, output_context->in.used);

	output_context->out.data = dup;
	output_context->out.used = output_context->in.used;
	output_context->out.free = 1;

	return SUCCESS;
}

PHP_MINIT_FUNCTION(myext)
{
	return SUCCESS;
}

PHP_RINIT_FUNCTION(myext)
{
	php_output_handler *handler;

	handler = php_output_handler_create_internal("myext handler", sizeof("myext handler") -1, myext_output_handler, /* PHP_OUTPUT_HANDLER_DEFAULT_SIZE */ 128, PHP_OUTPUT_HANDLER_STDFLAGS);

	php_output_handler_start(handler);

	return SUCCESS;
}

/* {{{ myext_module_entry
 */
zend_module_entry myext_module_entry = {
    STANDARD_MODULE_HEADER,
    "myext",
    NULL, /* Function entries */
    PHP_MINIT(myext), /* Module init */
    NULL, /* Module shutdown */
    PHP_RINIT(myext), /* Request init */
    NULL, /* Request shutdown */
    NULL, /* Module information */
    "0.1", /* Replace with version number for your extension */
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MYEXT
ZEND_GET_MODULE(myext)
#endif
