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

static const zend_function_entry myext_functions[] =
{
	PHP_FE(fahrenheit_to_celsius,arginfo_fahrenheit_to_celsius)
	PHP_FE(celsius_to_fahrenheit,arginfo_celsius_to_fahrenheit)
	PHP_FE(temperature_converter, arginfo_temperature_converter)

	PHP_FE(multiple_fahrenheit_to_celsius, arginfo_multiple_fahrenheit_to_celsius)

	PHP_FE_END
};


/* {{{ myext_module_entry
 */
zend_module_entry myext_module_entry = {
    STANDARD_MODULE_HEADER,
    "myext",
    myext_functions, /* Function entries */
    NULL, /* Module init */
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
	zend_long mode = 1;
	zend_string *result;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "d|l", &t, &mode) == FAILURE) {
		return;
	}

	switch (mode)
	{
		case 1:
			result = strpprintf(0, "%.2f degrees fahrenheit gives %.2f degrees celsius", t, php_fahrenheit_to_celsius(t));
			RETURN_STR(result);
		case 2:
			result = strpprintf(0, "%.2f degrees celsius gives %.2f degrees fahrenheit", t, php_celsius_to_fahrenheit(t));
			RETURN_STR(result);
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
	HashTable *temperatures;
	zval *data;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "h", &temperatures) == FAILURE) {
		return;
	}
	if (zend_hash_num_elements(temperatures) == 0) {
		return;
	}

	array_init_size(return_value, zend_hash_num_elements(temperatures));

	ZEND_HASH_FOREACH_VAL(temperatures, data)
		zval dup;
		ZVAL_COPY_VALUE(&dup, data);
		convert_to_double(&dup);
		add_next_index_double(return_value, php_fahrenheit_to_celsius(Z_DVAL(dup)));
	ZEND_HASH_FOREACH_END();
}
