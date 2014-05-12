zend_class_entry *ce_Logger;
zend_class_entry *ce_Logger_iface;
static zend_class_entry *ce_Logger_ex;

PHP_MINIT_FUNCTION(myext);
PHP_METHOD( Logger, __construct );
PHP_METHOD( Logger, log );
