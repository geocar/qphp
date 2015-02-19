
A usable, but mostly incomplete K client in PHP:

    $k=new K();
    print_r($k->k("{a::x;(\"foo\";x)}", 69));

Can also create symbols with `K::S()` e.g.

    print_r($k->k("{insert[x](`h`d`v!y)}", K::S("table"), array(array(42), array(69), array(38))));

Not yet implemented:

* Dicts
* Tables (no PHP support; array of dicts maybe? or an object?)
* Guids (no PHP support; object type?)
* Floats (awkward in PHP; strings?)
* 64bit Ints (incorrect in PHP; strings?)
* Exceptions (-128 just generates an exception; not the correct one)


