#ifndef PHP_MYEXT_H
#define PHP_MYEXT_H

static int myext_zend_extension_startup(zend_extension *ext);
static PHP_MINIT_FUNCTION(myext);

#endif
