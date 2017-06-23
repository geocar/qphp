<?php

require_once("k.inc");

class Receiver {
  public function ks($a) {}
  public function k($a) {
    if($a === "dude") return K::S("hello world");
    return 69;
  }
};


K::server_loop(1234, new Receiver());
