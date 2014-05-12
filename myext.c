#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_myext.h"
#include "SAPI.h"

/* {{{ myext_module_entry
 */
zend_module_entry myext_module_entry = {
    STANDARD_MODULE_HEADER,
    "myext",
    NULL, /* Function entries */
    NULL, /* Module init */
    NULL, /* Module shutdown */
    NULL, /* Request init */
    NULL, /* Request shutdown */
    PHP_MINFO(myext), /* Module information */
    "0.1", /* Replace with version number for your extension */
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MYEXT
ZEND_GET_MODULE(myext)
#endif

PHP_MINFO_FUNCTION(myext)
{
	const char custom_content_pattern[] = "<tr><td class='e'>Conference tutorial :</td><td class='v'><img src='%s'></td></tr>";
	char *custom_content = NULL;
	int size;

	php_info_print_table_start();
	php_info_print_table_header(2, "myext support", "enabled");
	php_info_print_table_row(2, "Temperature formula used :", "C = F - 32 * 5/9");
	if (!sapi_module.phpinfo_as_text) {
		spprintf(&custom_content, 0, custom_content_pattern, PHPINFO_IMG_URL);
		php_output_write(custom_content, strlen(custom_content));
		efree(custom_content);
	}
	php_info_print_table_end();
}
