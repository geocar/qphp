# KDB client for PHP

A usable, but mostly incomplete K client in PHP:

    $k=new K();
    print_r($k->k("{a::x;(\"foo\";x)}", 69));

Can also create symbols with `K::S()` e.g.

    print_r($k->k("{insert[x](`h`d`v!y)}", K::S("table"), array(array(42), array(69), array(38))));

# Reference Guide
## Symbol Construction

`K::S("text")` creates a symbol from a string. This is what you get when you type:

    `text

into KDB.

## Guid Construction

`K::G()` creates a guid.

## Instance Methods

* `$k->k()`
* `$k->ks()`

These send a statement (first argument). Optional arguments will be assigned to x, y, z, etc.

`k` will send a message, then wait for (and decode) the results.  `ks` will send a message and tell KDB
not to bother sending any results.

* `$k->ka()`
* `$k->kr()`

`ka` and `kr` work like the the send and receive legs of `k`. This allows you to pipeline messages:

   for($i=0;$i<100;++$i) $k->ka("1+$i");
   for($i=0;$i<100;++$i) $r[] = $k->kr();

# NYI

These things aren't implemented yet.

* 64bit Ints (requires 5.4 for native; strings?)

