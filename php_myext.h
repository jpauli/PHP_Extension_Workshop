zend_class_entry *ce_Logger;

PHP_MINIT_FUNCTION(myext);
PHP_METHOD( Logger, __construct );
PHP_METHOD( Logger, log );
