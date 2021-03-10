<?php
/**
 * this script holds the sink handlers. 
 * 
 */
class Sink
{

	/**
	 * negative taint inference
	 * finds the approximate match of input string in the output string, and returns
	 * a start index and a length on the output string for the matched part
	 * the third returned argument is the percentage of matching
	 */
	protected static function NTI($in,$out)
	{
	  $minDistance=PHP_INT_MAX;
	  $inSize=strlen($in);
	  $outSize=strlen($out);
	  $hardLimit=$inSize;

	  for ($i=$inSize;$i<$outSize;$i+=1)
	  {
	      $distance=levenshtein($in,substr($out,$i)/*,$hardLimit*/);
	      if ($distance<$minDistance)
	      {
	          $minDistance=$distance;
	          $result=substr($out,$i);
	          $resultIndex=$i;
	      }
	  }
	  // found the best start point, now finding the best end point
	  for ($i=$inSize;$i<strlen($result);++$i)
	  {
	      $distance=levenshtein($in,substr($result,0,$i)/*,$hardLimit*/);
	      //optimization: distance reduces until a certain point, and increases afterwards, break there
	      if ($distance>$minDistance) break;
	      $minDistance=$distance;
	      $resultLength=$i;
	  }
	  return array($resultIndex,$resultLength,1-$minDistance/$inSize);
	}
	static function Init()
	{
		$inputs=array();
		foreach ($_GET as $k=>$v)
			$inputs[0][$k]=$v;
		foreach ($_POST as $k=>$v)
			$inputs[1][$k]=$v;
		//might wanna add headers and cookies for better safety
		self::$logfile=self::GetLogFile("/tmp/logs/sql");
	}
	protected static $inputs;
	protected static $logfile;
	protected static function GetLogFile($file)
	{
		$list=glob($file."*");
		sort($list);

		$t=preg_match('/(\d+)\.{1,5}.*$/', end($list), $matches);
		if ($t)
			$number=$matches[1];
		else
			$number="0";
		$number=str_pad($number+1, 3,"0",STR_PAD_LEFT);
		return $file.$number.".txt";
	}
	protected static function DumpLog($in,$out,$GET=true)
	{
		$f=self::$file;
		if ($GET)
			$GET="GET";
		else
			$GET="POST";
		$data="IN {$GET} 1 ".strlen($in)." ".$in.PHP_EOL;
		file_put_contents($f, $data,FILE_APPEND);
		$data="OUT sql 1 ".strlen($out)." ".$out.PHP_EOL;
		file_put_contents($f, $data,FILE_APPEND);
	}
	static function Handler($query)
	{
		//put your sink handling logic here
		foreach (self::$inputs[0] as $input) //get
			if (is_array($input))
				foreach ($input as $in)
					self::DumpLog($in,$query);
			else
				self::DumpLog($input,$query);
		foreach (self::$inputs[1] as $input) //post
			if (is_array($input))
				foreach ($input as $in)
					self::DumpLog($in,$query,false);
			else
				self::DumpLog($input,$query,false);
		if (count(self::$inputs[0])==0 and count(self::$inputs[1])==0)
			self::DumpLog("",$query);
	}
}

class PDO_ extends PDO
{

	function exec($query=null)
	{
		$args=func_get_args();
		Sink::Handler($query);
 		$reflector = new ReflectionClass(get_class($this));
        $parent = $reflector->getParentClass();
        $method = $parent->getMethod('exec');
        return $method->invokeArgs($this, $args);
	}
	function query($query=null)
	{
		$args=func_get_args();
		Sink::Handler($query);
 		$reflector = new ReflectionClass(get_class($this));
        $parent = $reflector->getParentClass();
        $method = $parent->getMethod('query');
        return $method->invokeArgs($this, $args);
      }
}
class mysqli_ extends mysqli
{
	function query($query=null)
	{
		$args=func_get_args();
		Sink::Handler($query);
 		$reflector = new ReflectionClass(get_class($this));
        $parent = $reflector->getParentClass();
        $method = $parent->getMethod('query');
        return $method->invokeArgs($this, $args);
	}
	function real_query($query=null)
	{
		$args=func_get_args();
		Sink::Handler($query);
 		$reflector = new ReflectionClass(get_class($this));
        $parent = $reflector->getParentClass();
        $method = $parent->getMethod('real_query');
        return $method->invokeArgs($this, $args);
	}
	function multi_query($query=null)
	{
		$args=func_get_args();
		Sink::Handler($query);
 		$reflector = new ReflectionClass(get_class($this));
        $parent = $reflector->getParentClass();
        $method = $parent->getMethod('multi_query');
        return $method->invokeArgs($this, $args);
	}
}

function mysql_query_($query=null)
{
	$args=func_get_args();
	Sink::Handler($query);
	return call_user_func_array("mysql_query", $args);
}
function mysqli_query_($link=null,$query=null)
{
	$args=func_get_args();
	Sink::Handler($query);
	return call_user_func_array("mysql_query", $args);
}
function mysqli_real_query_($link=null,$query=null)
{
	$args=func_get_args();
	Sink::Handler($query);
	return call_user_func_array("mysql_query", $args);
}
function mysqli_multi_query_($link=null,$query=null)
{
	$args=func_get_args();
	Sink::Handler($query);
	return call_user_func_array("mysql_query", $args);
}
function mysql_db_query_($dbname=null,$query=null)
{
	$args=func_get_args();
	Sink::Handler($query);
	return call_user_func_array("mysql_query", $args);
}
