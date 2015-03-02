<?php
class KException extends Exception {};
class K {
  private static $be = -1;
  private $h;

  public static function init() {
    if(self::$be === -1) {
      self::$be = (pack("N",-69) === pack("l",-69));
    }
  }

  public function __construct($h="localhost",$p=1234,$up="anonymous") {
    self::init();

    $this->h = fsockopen($h,$p);
    fwrite($this->h, $up.pack("cc", 1,0));
    if(ord(fgetc($this->h)) != 1) throw new Exception("Can't connect to k: $h:$p");
  }
  function __destruct() {
    fclose($this->h);
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
    $a=unpack("l2", fread($this->h,8));
    $x=fread($this->h,$a[2]-8).pack('llll',0,0,0,0);// arthur doesn't pad
    $h=fopen("php://memory", "r+b"); fwrite($h,$x); rewind($h); $o=self::dec($h); fclose($h);
    return $o;
  }

  public static function S($x) {
    $s = pack("c", -11) . $x . pack("c", 0);
    return (object)array("s" => $s);
  }
  public static function G($x=null) {
    if($x === null) {
      if(function_exists('openssl_random_pseudo_bytes')) $x = openssl_random_pseudo_bytes(16);
      else{$x="";for($i=0;$i<16;++$i)$x.=chr(mt_rand(0,255));}
    } else { $x = hex2bin(str_replace("-","",$x)); }
    return (object)array("s" => pack("c", -2) . $x);
  }

  private static function aa($x) {
    $n=min(100,count($x));for($i=0;$i<$n;++$i)if(!isset($x[$i]))return true;
    return false;
  }

  private static function e1($x) {
    if(is_null($x)){return pack("cc",101,0);}
    if($x === true){return pack("cc",-1,1);}
    if($x === false){return pack("cc",-1,0);}
    if(is_string($x)){return pack("ccl",10,0,strlen($x)).$x;}
    if(is_integer($x)){return pack("cl",-6,$x);}
    if(is_float($x)){return pack("cd",-9,$x);}
    if(is_object($x)){return $x->s;}
    if(!is_array($x)){throw new Exception("Can't encode this: " . var_export($x));}
    $a="";
    if(self::aa($x)){//associative array->dict
      $n=count($x);$a=pack("cccl",99,0,0,$n);$v=array();
      foreach($x as $k => $_){$a.=pack("c",-11).$k.pack("c",0);$v[]=$_;}
      $x=$v;//fall through
    }
    $a.=pack("ccl",0,0,count($x));foreach($x as $b){$a.=self::e1($b);}
    return $a;
  }
  private static function enc($b,$x) {
    $x=self::e1($x);
    return pack("ccccl",!self::$be,$b,0,0,8+strlen($x)).$x;
  }
  private static function flip(&$c, &$x) {
    foreach($x as$k=>$a)foreach($a as$i=>$v)$c[$i][$k]=$v;
  }
  private static function om(&$c) {
    $o=array();foreach($c as$i=>$x)$o[$i]=(object)$x;return $o;
  }
  private static function d1($t,$x) {
    switch($t){
    case 0:return self::dec($x);
    case 1:$n=ord(fgetc($x));return $n==1;
    case 4:$n=ord(fgetc($x));return $n;
    case 5:$a=unpack("s",fread($x,2));return $a[1];
    case 6:$a=unpack("l",fread($x,4));return $a[1];
    case 7:$a=unpack("l2",fread($x,8));if($a[2]==-1&&$a[1]<0)return $a[1];if($a[2])throw new Exception("Number too long(64)");return $a[1];
    case 8:$a=unpack("f",fread($x,8));return $a[1];
    case 9:$a=unpack("d",fread($x,8));return $a[1];
    case 10:return fgetc($x);
    case 11:$s="";while(ord($c=fgetc($x)))$s.=$c;return $s;
    case 98:fgetc($x);$k=self::dec($x);$v=self::dec($x);$c=array();self::flip($c,$k);self::flip($c,$v);return self::om($c);
    case 99:$k=self::dec($x);$v=self::dec($x);return array_combine($k,$v);
    case 101:return null;
    case 128:throw new KException(self::d1(11,$x));
    default:throw new Exception("Can't decode this $t yet");
    } 
  }
  private static function dec($x) {
    $t=ord(fgetc($x));
    if($t>=128)return self::d1(-($t-256),$x);
    if($t==99||$t==98)return self::d1($t,$x);
    if(ord(fgetc($x)) != 0)throw new Exception("Protocol error");
    $n=self::d1(6,$x);
    if($t===10)return fread($x,$n);
    $a=array();
    for($i=0;$i<$n;++$i)$a[]=self::d1($t,$x);
    return $a;
  }
}

