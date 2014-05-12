zend_class_entry *ce_Logger;
zend_class_entry *ce_Logger_iface;

PHP_MINIT_FUNCTION(myext);
PHP_METHOD( Logger, __construct );
PHP_METHOD( Logger, log );
