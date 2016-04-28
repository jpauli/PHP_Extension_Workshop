#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "Zend/zend_extensions.h"
#include "php_myext.h"

zend_module_entry myext_module_entry = {
    STANDARD_MODULE_HEADER,
    "myext",
    NULL, /* Function entries */
    PHP_MINIT(myext), /* Module init */
    NULL, /* Module shutdown */
    NULL, /* Request init */
    NULL, /* Request shutdown */
    NULL, /* Module information */
    "0.1", /* Replace with version number for your extension */
    STANDARD_MODULE_PROPERTIES
};

zend_extension_version_info extension_version_info = {
	ZEND_EXTENSION_API_NO,
	ZEND_EXTENSION_BUILD_ID
};

zend_extension zend_extension_entry = {
	"myext-zend-extension",
	"1.0",
	"Author",
	"http://www.me",
	"copyright",
	myext_zend_extension_startup,
	NULL,
	NULL,
	NULL,
	NULL,

	myext_zend_extension_op_array_handler,
	myext_zend_extension_statement_handler,
	myext_zend_extension_fcall_begin_handler,
	myext_zend_extension_fcall_end_handler,
	myext_zend_extension_op_array_ctor,
	myext_zend_extension_op_array_dtor,

	STANDARD_ZEND_EXTENSION_PROPERTIES
};

static int myext_zend_extension_startup(zend_extension *ext)
{
	php_printf("Loading zend_extension %s\n", ext->name);

	return zend_startup_module(&myext_module_entry);
}

static PHP_MINIT_FUNCTION(myext)
{
	php_printf("Loading php extension %d\n", module_number);

	return SUCCESS;
}

static void myext_zend_extension_op_array_handler(zend_op_array *op_array)
{
	if (op_array->function_name) {
		op_array->reserved[0] = zend_string_copy(op_array->function_name);
	}
}

static void myext_zend_extension_statement_handler(zend_op_array *op_array)
{

}

static void myext_zend_extension_fcall_begin_handler(zend_op_array *op_array)
{

}

static void myext_zend_extension_fcall_end_handler(zend_op_array *op_array)
{

}

static void myext_zend_extension_op_array_ctor(zend_op_array *op_array)
{

}

static void myext_zend_extension_op_array_dtor(zend_op_array *op_array)
{
	if (op_array->reserved[0]) {
		php_printf("Destroying OP Array for function '%s'\n",ZSTR_VAL((zend_string *)op_array->reserved[0]));
		zend_string_release(op_array->reserved[0]);
	} else {
		php_printf("Destroying OP Array for file '%s'", ZSTR_VAL(op_array->filename));
	}
	php_printf("\n");
}


/* }}} */
/*
 * Comment this to prevent PHP extension loading
 *
#ifdef COMPILE_DL_MYEXT
ZEND_GET_MODULE(myext)
#endif
*/
