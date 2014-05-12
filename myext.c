#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_myext.h"

ZEND_BEGIN_ARG_INFO_EX(arginfo_fahrenheit_to_celsius, 0, 0, 1)
    ZEND_ARG_INFO(0, fahrenheit)
ZEND_END_ARG_INFO();
ZEND_BEGIN_ARG_INFO_EX(arginfo_celsius_to_fahrenheit, 0, 0, 1)
    ZEND_ARG_INFO(0, celsius)
ZEND_END_ARG_INFO();
ZEND_BEGIN_ARG_INFO_EX(arginfo_temperature_converter, 0, 0, 1)
    ZEND_ARG_INFO(0, temperature)
    ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_multiple_fahrenheit_to_celsius, 0, 0, 1)
    ZEND_ARG_INFO(0, temperatures)
ZEND_END_ARG_INFO();

zend_function_entry myext_functions[] =
{
	PHP_FE(fahrenheit_to_celsius,arginfo_fahrenheit_to_celsius)
	PHP_FE(celsius_to_fahrenheit,arginfo_celsius_to_fahrenheit)
	PHP_FE(temperature_converter, arginfo_temperature_converter)

	PHP_FE(multiple_fahrenheit_to_celsius, arginfo_multiple_fahrenheit_to_celsius)

	PHP_FE_END
};

PHP_MINIT_FUNCTION(myext)
{
	REGISTER_LONG_CONSTANT("TEMP_CONVERTER_TO_CELSIUS", TEMP_CONVERTER_TO_CELSIUS, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TEMP_CONVERTER_TO_FAHRENHEIT", TEMP_CONVERTER_TO_FAHRENHEIT, CONST_CS|CONST_PERSISTENT);
}

/* {{{ myext_module_entry
 */
zend_module_entry myext_module_entry = {
    STANDARD_MODULE_HEADER,
    "myext",
    myext_functions, /* Function entries */
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

static double php_fahrenheit_to_celsius(double f)
{
	return ((double)5/9) * (double)(f - 32);
}

static double php_celsius_to_fahrenheit(double c)
{
	return (((double)9/5) * c) + 32 ;
}

PHP_FUNCTION(temperature_converter)
{
	double t;
	long mode = 1;
	char *result = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "d|l", &t, &mode) == FAILURE) {
		return;
	}

	switch (mode)
	{
		case TEMP_CONVERTER_TO_CELSIUS:
			spprintf(&result, 0, "%.2f degrees fahrenheit gives %.2f degrees celsius", t, php_fahrenheit_to_celsius(t));
			RETURN_STRINGL(result, strlen(result), 0);
		case TEMP_CONVERTER_TO_FAHRENHEIT:
			spprintf(&result, 0, "%.2f degrees celsius gives %.2f degrees fahrenheit", t, php_celsius_to_fahrenheit(t));
			RETURN_STRINGL(result, strlen(result), 0);
		default:
			php_error(E_WARNING, "Invalid mode provided, accepted values are 1 or 2");
	}
}

PHP_FUNCTION(fahrenheit_to_celsius)
{
	double f;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &f) == FAILURE) {
		return;
	}

	RETURN_DOUBLE(php_fahrenheit_to_celsius(f));
}

PHP_FUNCTION(celsius_to_fahrenheit)
{
	double c;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &c) == FAILURE) {
		return;
	}

	RETURN_DOUBLE(php_celsius_to_fahrenheit(c));
}

PHP_FUNCTION(multiple_fahrenheit_to_celsius)
{
	HashTable *temperatures = NULL;
	HashPosition pos;
	zval **data = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "h", &temperatures) == FAILURE) {
		return;
	}
	if (zend_hash_num_elements(temperatures) == 0) {
		return;
	}

	array_init_size(return_value, zend_hash_num_elements(temperatures));

	for (zend_hash_internal_pointer_reset_ex(temperatures, &pos);
		 zend_hash_get_current_data_ex(temperatures, (void **)&data, &pos) != FAILURE;
		 zend_hash_move_forward_ex(temperatures, &pos)) {
			if (Z_TYPE_PP(data) != IS_DOUBLE) {
				convert_to_double_ex(data);
			}
			add_next_index_double(return_value, php_fahrenheit_to_celsius(Z_DVAL_PP(data)));
	}
}
