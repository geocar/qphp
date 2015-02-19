<?php

class K {
  private $h;

  public function __construct($h="localhost",$p=1234,$up="anonymous") {
    $this->h = fsockopen($h,$p);
    fwrite($this->h, $up.pack("cc", 1,0));
    if(ord(fgetc($this->h)) != 1) throw new Exception("Can't connect to k: $h:$p");
  }

  public function ks($s) {
    if(func_num_args()===1){$x=self::enc(0,$s);}else{$x=self::enc(0,func_get_args());}
    fwrite($this->h,$x);
  }
  public function k($s) {
    if(func_num_args()===1){$x=self::enc(1,$s);}else{$x=self::enc(1,func_get_args());}
    fwrite($this->h,$x);
    return $this->kr();
  }
  public function ka($s) {
    if(func_num_args()===1){$x=self::enc(1,$s);}else{$x=self::enc(1,func_get_args());}
    fwrite($this->h,$x);
  }
  public function kr() {
    $a=unpack("V2", fread($this->h,8));
    $x=fread($this->h,$a[2]-8).pack('VVVV',0,0,0,0);// arthur doesn't pad
    $h=fopen("php://memory", "r+b"); fwrite($h,$x); rewind($h); $o=self::dec($h); fclose($h);
    return $o;
  }

  public static function S($x) {
    $s = pack("c", -11) . $x . pack("c", 0);
    return (object)array("s" => $s);
  }

  private static function e1($x) {
    if(is_null($x)){return pack("cc",101,0);}
    if($x === true){return pack("cc",-1,1);}
    if($x === false){return pack("cc",-1,0);}
    if(is_string($x)){return pack("ccV",10,0,strlen($x)).$x;}
    if(is_integer($x)){return pack("cV",-6,$x);}
    if(is_array($x)){$a=pack("ccV",0,0,count($x));foreach($x as $b){$a.=self::e1($b);}return $a;}
    if(is_object($x)){return $x->s;}
    throw new Exception("Can't encode this: " . var_export($x));
  }
  public static function enc($b,$x) {
    $x=self::e1($x);
    return pack("ccccV",1,$b,0,0,8+strlen($x)).$x;
  }

  private static function d1($t,$x) {
    switch($t){
    case 0:return self::dec($x);
    case 1:$n=ord(fgetc($x));return $n==1;
    case 4:$n=ord(fgetc($x));return $n;
    case 5:$a=unpack("i",fread($x,2));return $a[1];
    case 6:$a=unpack("V",fread($x,4));return $a[1];
    case 7:$a=unpack("V2",fread($x,8));if($a[2])throw new Exception("Number too long(64)");return $a[1];
    case 10:return fgetc($x);
    case 11:$s="";while(ord($c=fgetc($x)))$s.=$c;return $s;
    case 101:return null;
    default:throw new Exception("Can't decode this $t yet");
    } 
  }

  public static function dec($x) {
    $t=ord(fgetc($x));
    if($t>=128)return self::d1(-($t-256),$x);
    if(ord(fgetc($x)) != 0)throw new Exception("Protocol error");
    $n=self::d1(6,$x);
    if($t===10)return fread($x,$n);
    $a=array();
    for($i=0;$i<$n;++$i)$a[]=self::d1($t,$x);
    return $a;
  }
}
  
//$k=new K();
//print_r($k->k("{a::x}", K::S("whateveR")));
//    print_r($k->k("k){a::`h`d`v!x}", array(42, 69, 38)));



