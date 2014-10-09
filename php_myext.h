
#ifndef PHP_MYEXT_H
#define PHP_MYEXT_H

static int myext_zend_extension_startup(zend_extension *ext);
static PHP_MINIT_FUNCTION(myext);

static void myext_zend_extension_op_array_handler(zend_op_array *op_array);
static void myext_zend_extension_statement_handler(zend_op_array *op_array);
static void myext_zend_extension_fcall_begin_handler(zend_op_array *op_array);
static void myext_zend_extension_fcall_end_handler(zend_op_array *op_array);
static void myext_zend_extension_op_array_ctor(zend_op_array *op_array);
static void myext_zend_extension_op_array_dtor(zend_op_array *op_array);

#endif
