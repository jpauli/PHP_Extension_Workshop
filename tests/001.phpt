--TEST--
Test temperature converter
--SKIPIF--
<?php if (!extension_loaded("myext")) print "skip"; ?>
--FILE--
<?php 
printf("%.2f", fahrenheit_to_celsius(70));
echo "\n";
echo celsius_to_fahrenheit(20);
echo "\n";
echo temperature_converter(20, 2);
?>

--EXPECTF--
21.11
68
20.00 degrees celsius gives 68.00 degrees fahrenheit