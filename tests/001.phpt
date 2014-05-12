--TEST--
Test for information provided by phpinfo();
--SKIPIF--
<?php if (!extension_loaded("myext")) print "skip"; ?>
--FILE--
<?php 
(new Reflectionextension('myext'))->info();
?>
--EXPECTF--
myext

%s
Temperature formula used : => C = F - 32 * 5/9