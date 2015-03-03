<?php

require_once("k.inc");

$k=new K("localhost", 1234);
var_dump($b=$k->k("a"));
