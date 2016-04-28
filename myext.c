#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_myext.h"
#include "ext/spl/spl_exceptions.h"
#include "Zend/zend_exceptions.h"

/* {{{ myext_module_entry
 */
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
/* }}} */

#ifdef COMPILE_DL_MYEXT
ZEND_GET_MODULE(myext)
#endif

static int Logger_res_id;
static zval dummy;

#define LOG_ERROR 0
#define LOG_DEBUG 1
#define LOG_INFO 2

#define LOGGER_RESOURCE_NAME "Logger internal resource"
#define LOGGER_RESOURCE_ID Logger_res_id

#define LOGGER_DEFAULT_LOG_FILE "/tmp/php-extension-logger.log"

zend_class_entry *ce_Logger;
zend_class_entry *ce_Logger_iface;
static zend_class_entry *ce_Logger_ex;

ZEND_BEGIN_ARG_INFO(arginfo_logger___construct, 0)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_logger_log, 0, 0, 2)
    ZEND_ARG_INFO(0, level)
    ZEND_ARG_INFO(0, message)
ZEND_END_ARG_INFO()

static zend_function_entry logger_class_functions[] = {
    PHP_ME( Logger, __construct, arginfo_logger___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR )
    PHP_ME( Logger, log, arginfo_logger_log, ZEND_ACC_PUBLIC )
    PHP_FE_END
};

static zend_function_entry logger_iface_functions[] = {
    PHP_ABSTRACT_ME( LoggerInterface, log, arginfo_logger_log )
    PHP_FE_END
};

PHP_MINIT_FUNCTION(myext)
{
    zend_class_entry ce, ce_iface, ce_exception;

    INIT_CLASS_ENTRY(ce_exception, "LoggerException", NULL);
    ce_Logger_ex = zend_register_internal_class_ex(&ce_exception, zend_exception_get_default());

    INIT_CLASS_ENTRY(ce_iface, "LoggerInterface", logger_iface_functions);
    ce_Logger_iface = zend_register_internal_interface(&ce_iface);

    INIT_CLASS_ENTRY(ce, "Logger", logger_class_functions);
    ce_Logger = zend_register_internal_class(&ce);
    zend_class_implements(ce_Logger, 1, ce_Logger_iface);
    
    zend_declare_property_string(ce_Logger, ZEND_STRL("file"), "", ZEND_ACC_PROTECTED);
    zend_declare_property_null(ce_Logger, ZEND_STRL("handle"), ZEND_ACC_PRIVATE);
    zend_declare_class_constant_long(ce_Logger, ZEND_STRL("INFO"), LOG_INFO);
    zend_declare_class_constant_long(ce_Logger, ZEND_STRL("DEBUG"), LOG_DEBUG);
    zend_declare_class_constant_long(ce_Logger, ZEND_STRL("ERROR"), LOG_ERROR);

    LOGGER_RESOURCE_ID = zend_register_list_destructors_ex(logger_rsrc_dtor, NULL, LOGGER_RESOURCE_NAME, module_number);

    return SUCCESS;
}

PHP_METHOD( Logger, __construct )
{
    char *file = NULL;
    size_t file_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &file, &file_len) == FAILURE) {
        return;
    }

    if (!file) {
        file     = LOGGER_DEFAULT_LOG_FILE;
        file_len = strlen(LOGGER_DEFAULT_LOG_FILE);
    }

    zend_update_property_stringl(ce_Logger, getThis(), ZEND_STRL("file"), file, file_len);
}

PHP_METHOD( Logger, log )
{
    long level;
    char *message, *level_str, *message_str;
    size_t message_len, message_str_len;
    zval *handle, new_handle;
    php_stream *fd;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &level, &message, &message_len) == FAILURE) {
        return;
    }

    handle = zend_read_property(ce_Logger, getThis(), ZEND_STRL("handle"), 1, &dummy);

    switch(Z_TYPE_P(handle))
    {
        case IS_NULL:
        	fd = php_stream_open_wrapper(Z_STRVAL_P(zend_read_property(ce_Logger, getThis(), ZEND_STRL("file"), 0, &dummy)), "ab", 0, NULL);
            if (!fd ) {
        		zend_throw_exception(spl_ce_RuntimeException, "Cannot open file", 0);
        		return;
            }
            {
            	zend_resource *res;
            	res = zend_register_resource((void *)fd, LOGGER_RESOURCE_ID);
            	ZVAL_RES(&new_handle, res);
            	zend_update_property(ce_Logger, getThis(), ZEND_STRL("handle"), &new_handle);
            }
            break;
        case IS_RESOURCE:
            fd = (php_stream *)zend_fetch_resource2(Z_RES_P(handle), LOGGER_RESOURCE_NAME, LOGGER_RESOURCE_ID, -1);
            break;
        default:
            zend_error_noreturn(E_ERROR, "Something really wrong happened here, type of 'file' property cannot be something else than NULL or a resource.");
    }

    switch(level)
    {
        case LOG_ERROR:
            level_str = "ERROR";
            break;
        case LOG_DEBUG:
            level_str = "DEBUG";
            break;
        case LOG_INFO:
            level_str = "INFO";
            break;
        default:
            zend_throw_exception(NULL, "Internal Logger error", 0);
            return;
    }

	message_str_len = spprintf(&message_str, 0, "[%s]:%*s", level_str, message_len, message);
    php_stream_write(fd, message, message_str_len);
    efree(message_str);

    RETURN_TRUE
}

static void logger_rsrc_dtor(zend_resource *rsrc)
{
	php_stream_close((php_stream *)rsrc->ptr);
}
