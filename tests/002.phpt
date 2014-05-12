--TEST--
Test temperature converter array
--SKIPIF--
<?php if (!extension_loaded("myext")) print "skip"; ?>
--FILE--
<?php 
$temps = array(68, 77, 78.8);
var_dump(multiple_fahrenheit_to_celsius($temps));
?>
--EXPECTF--
array(3) {
  [0]=>
  float(20)
  [1]=>
  float(25)
  [2]=>
  float(26)
}