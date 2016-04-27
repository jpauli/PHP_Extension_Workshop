zend_class_entry *ce_Logger;
static void logger_rsrc_dtor(zend_resource *rsrc);

PHP_MINIT_FUNCTION(myext);
PHP_METHOD( Logger, __construct );
PHP_METHOD( Logger, log );
