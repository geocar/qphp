# KDB IPC implementation for PHP

## KDB Client

A usable, but mostly incomplete K client in PHP:

    $k=new K();
    print_r($k->k("{a::x;(\"foo\";x)}", 69));

Can also create symbols with `K::S()` e.g.

    print_r($k->k("{insert[x](`h`d`v!y)}", K::S("table"), array(array(42), array(69), array(38))));

## KDB Server

    class Receiver {
      public function ks($a, $x=null, $y=null, $z=null) {} // called for async
      public function k($a , $x=null, $y=null, $z=null) {return K::S("table");} // called for sync
    };
    K::server_loop(1234, new Receiver());

# Reference Guide
## Symbol Construction

`K::S("text")` creates a symbol from a string. This is what you get when you type:

    `text

into KDB.

## Guid Construction

`K::G()` creates a guid.

## Instance Methods

* `$k->k(...)`
* `$k->ks(...)`

These send a statement (first argument). Optional arguments will be assigned to x, y, z, etc.

`k` will send a message, then wait for (and decode) the results.  `ks` will send a message and tell KDB
not to bother sending any results.

* `$k->ka(...)`
* `$k->kr()`

`ka` and `kr` work like the the send and receive legs of `k`. This allows you to pipeline messages:

    for($i=0;$i<100;++$i) $k->ka("1+", $i);
    for($i=0;$i<100;++$i) $r[] = $k->kr();

## Read from String

    $r = $k->krs("<ipc message>");

If you have an IPC message in a string (for example, you read it from a file) then you can parse it directly.

# Opinionated TLS mode

If you're using kdb+ in TLS mode, you can connect if you supply the CA certificate so that qphp can verify it:

    $k = new KTLS("ca.pem","localhost",1234);
    print_r($k->k("2+2"));

You can also implement a TLS server:

    KTLS::server_loop(array("ssl" => array("local_cert" => "server-crt.pem", "local_pk" => "server-key.pem")),
      1234, new Receiver());

