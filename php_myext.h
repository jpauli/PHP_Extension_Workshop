static double php_celsius_to_fahrenheit(double);
static double php_fahrenheit_to_celsius(double);

PHP_FUNCTION(fahrenheit_to_celsius);
PHP_FUNCTION(celsius_to_fahrenheit);
PHP_FUNCTION(temperature_converter);

PHP_FUNCTION(multiple_fahrenheit_to_celsius);
PHP_MINIT_FUNCTION(myext);

#define TEMP_CONVERTER_TO_FAHRENHEIT 2
#define TEMP_CONVERTER_TO_CELSIUS 1
