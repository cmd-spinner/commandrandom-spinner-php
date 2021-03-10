<?php
require_once __DIR__."/vendor/autoload.php";

use PhpParser\Error;
use PhpParser\NodeDumper;
use PhpParser\ParserFactory;
use PhpParser\Node;
use PhpParser\Node\Stmt\Function_;
use PhpParser\NodeTraverser;
use PhpParser\NodeVisitorAbstract;
use PhpParser\Node\Scalar\String_;
use PhpParser\Node\Scalar\EncapsedStringPart;
use PhpParser\Node\Scalar\Encapsed;
use PhpParser\Node\Expr\FuncCall;
use PhpParser\Node\Expr\Assign;
use PhpParser\node\expr\BinaryOp\Concat;
use PhpParser;
use Psalm\Codebase;
use Psalm\CodeLocation;
use Psalm\Context;
use Psalm\FileManipulation;
use Psalm\Plugin\Hook\AfterExpressionAnalysisInterface;
use Psalm\StatementsSource;
use Psalm\Type\TaintKindGroup;
require_once 'PHPCallGraph.php';
// use PhpParser\NodeVisitor\ParentConnectingVisitor;
// set up include path
set_include_path(
    // phpCallGraph
    realpath(dirname(__FILE__) . '/../src') . PATH_SEPARATOR
    // eZ Components
    . realpath(dirname(__FILE__) . '/../lib/ezcomponents') . PATH_SEPARATOR
    // other external libraries, e.g. Static Reflection
    . realpath(dirname(__FILE__) . '/../lib') . PATH_SEPARATOR
    // original include path from php.ini
    . get_include_path() . PATH_SEPARATOR
    // PEAR files shipped with phpCallGraph (only used if no PEAR installed)
    . realpath(dirname(__FILE__) . '/../lib/pear')
);
require_once 'autoload-ezcomponents.php';
require_once 'PHPCallGraphCli.php';
require_once 'node_analyzer'
use PhpParser\NodeVisitor\NodeConnectingVisitor;
$forward_tree = [];
$backward_tree = [];
$current_node;
class sink_function_finder extends NodeVisitorAbstract
{
    public function leaveNode(Node $node)
    {
        global $cmd_sink_functions;
        global $dumper;
        if($node instanceof FuncCall)
        {
            $name = $node->name;
            $result = in_array($name,$cmd_sink_functions);
            if($result == True)
            {
                $backward_node = new backward_unit($backward_node,0);
                array_push($backward_trace_node,0);
            }

        }
    }
}
class forward_unit
{
    public $node;
    public $forward_node = [];
    function __construct($node,$forward_node) 
    {
        $this->node = $node;
        $this->forward_node = $forward_node;
    }

}
class backward_unit
{
    public $node;
    public $backward_node = [];
    function __construct($node,$backward_node) 
    {
        $this->node = $node;
        $this->backward_node = $backward_node;
    }

}
function backward_trace_node($node)
{
    $backward_traverser = new NodeTraverser();
    $forward_traverser->addVisitor(new NodeConnectingVisitor);
    $forward_traverser->addVisitor(new variablee_Analyzer);
    $forward_traverser->traverse($node);

}
function forward_trace_node($node)
{
    global $current_ast;
    $parent_node = $node->getattribute("parent");
    if( $parent_node instanceof Assign)
    {
        echo "we find one assign operation"."\n";
        $variable_name = $parent_node->var->name;

    }
    echo "get into forward trace node"."\n";

}
function in_sink_function($node)
{
    global $cmd_sink_functions;
    global $dumper;
    global $current_arg;
    $parent_node = $node->getAttribute('parent');
    if(is_null($parent_node))
    {
        return False;
    }
    if($parent_node instanceof FuncCall)
    {
        $name = $parent_node->name;
        
        $result = in_array($name,$cmd_sink_functions);

        if($result == True)
        {
            if(($parent_node->args[0] instanceof String_ && $parent_node->args[0]->value == $current_arg)||($parent_node->args[0]->value instanceof Concat && $parent_node->args[0]->value->left->value == $current_arg))
            {
                return $parent_node;
            }
            elseif($parent_node->args[0]->value instanceof Encapsed && $parent_node->args[0]->value->parts[0] instanceof EncapsedStringPart && $parent_node->args[0]->value->parts[0]->value == $current_arg)
            {
                return $parent_node;
            }
            else
            {
                return False;
            }
        }
        else
        {
            return False;
        }
    }
    else
    {
        $tmp_result = in_sink_function($parent_node);
    }
    return $tmp_result;
}
function getAllPhpFiles($Path,$exclude=array())
{
	$Directory = new RecursiveDirectoryIterator($Path);
	$Iterator = new RecursiveIteratorIterator($Directory);
	$list=[];
	foreach ($Iterator as $it)
		if (pathinfo($it->getPathName(), PATHINFO_EXTENSION)=="php")
			if (!in_array(basename($it->getPathName()),$exclude)) //don't list your own script file!
				$list[]=$it->getPathName();
	return $list;
}
function get_cmd_list()
{
    $file_names = [];
    $folders = ["/bin","/sbin","/usr/bin","/usr/sbin"];
    foreach($folders as $folder)
    {
        $files = array_diff(scandir($folder),array(".",".."));
        $file_names = array_merge($files,$file_names);
    }
    
    return $file_names;
}

$phpcg = new PHPCallGraph($driver);
$phpcg->setShowInternalFunctions($phpfunctionsOption->value);        
$phpcg->setDebug($debugOption->value);
$phpcg->setAutoloadFile($autoloadOption->value);

// start generation
$phpcg->parse($input->getArguments(), $recursiveOption->value);
class variablee_Analyzer extends PHPParser_NodeVisitorAbstract
{
    public $results=array();
    public $total=array();
    public $nodeCount=0;
    static $sinkFunctions=array();
    static $sinkClasses=array();

    static $taintFunctions=array();
    function __construct($node_list)
    {
        if (self::$sinkFunctions===array())
            self::$sinkFunctions=explode(PHP_EOL,file_get_contents(__DIR__."/../data/sink-functions.txt"));
        if (self::$sinkClasses===array())
            self::$sinkClasses=explode(PHP_EOL,file_get_contents(__DIR__."/../data/sink-classes.txt"));
        if (self::$taintFunctions===array())
        {
            $out=[];
            $data=explode(PHP_EOL,file_get_contents(__DIR__."/../data/taint-functions.txt"));
            while ($data[0][0]=='#')
                array_shift($data); //comment line
            foreach ($data as $v)
            {
                if (!$v) continue;
                $x=preg_split("/\s+/",$v);
                self::$taintFunctions[$x[0]]=array('impact'=>$x[1],'tracking'=>$x[2],'inference'=>$x[3]);
            }
        }
    }


    protected function hasVariable( $arg)
    {
        if ($arg instanceof PhpParser\Node\Arg)
            return $this->hasVariable($arg->value);
        elseif ($arg instanceof PhpParser\Node\Expr\Variable)
            return true;
        elseif ($arg instanceof PhpParser\Node\Scalar)
            return false;
        elseif ($arg instanceof PhpParser\Node\Expr\FuncCall)
        {
            $res=false;
            for ($i=0;$i<count($arg->args);++$i)
                $res|=$this->hasVariable($arg->args[$i]);
            return $res;
        }
        elseif ($arg instanceof PhpParser\Node\Expr)
            return $this->hasVariable($arg->left)
                   || $this->hasVariable($arg->right);

    }
    public function leaveNode(PHPParser_Node $node) {
        $this->nodeCount++;
        if ($node instanceof PHPParser_Node_Stmt_Class or $node instanceof PhpParser\Node\Expr\New_) # class definition, check for extends
        {
            if ($node instanceof PHPParser_Node_Stmt_Class and isset($node->extends))
                $baseClass=$node->extends->parts[0];
            elseif ($node instanceof PhpParser\Node\Expr\New_ and isset($node->class))
                $baseClass=$node->class->parts[0];
            else
                $baseClass="";
            if ($baseClass)
            {
                if (in_array($baseClass, self::$sinkClasses))
                {
                    if (isset($this->results[$this->file]['sinks'][$baseClass]))
                        $this->results[$this->file]['sinks'][$baseClass]++;
                    else
                        $this->results[$this->file]['sinks'][$baseClass]=1;
                    if (isset($this->total['sinks'][$baseClass]))
                        $this->total['sinks'][$baseClass]++;
                    else
                        $this->total['sinks'][$baseClass]=1;
                }
            }
        }
        elseif ($node instanceof PHPParser_Node_Expr_FuncCall) # function call
        {
            if (isset($node->name) && isset($node->name->parts))
            {
                $functionName=$node->name->parts[0];
                if (in_array($functionName, self::$sinkFunctions))
                {
                    if (isset($this->results[$this->file]['sinks'][$functionName]))
                        $this->results[$this->file]['sinks'][$functionName]++;
                    else
                        $this->results[$this->file]['sinks'][$functionName]=1;
                    if (isset($this->total['sinks'][$functionName]))
                        $this->total['sinks'][$functionName]++;
                    else
                        $this->total['sinks'][$functionName]=1;
                }
                if (isset(self::$taintFunctions[$functionName]))
                {
                    if ($this->hasVariable($node))
                    {
                        //without any variables, this function is not propagating taint
                        if (isset($this->results[$this->file]['taint'][$functionName]))
                            $this->results[$this->file]['taint'][$functionName]++;
                        else
                            $this->results[$this->file]['taint'][$functionName]=1;
                        if (isset($this->total['taint'][$functionName]))
                            $this->total['taint'][$functionName]++;
                        else
                            $this->total['taint'][$functionName]=1;
                    }
                }
            }
        }
    }
    function process($path)
    {
        $this->nodeCount=0;
        $this->files=$files=(getAllPhpFiles($path));

        $parser = new PHPParser_Parser(new PHPParser_Lexer);    
        $traverser     = new PHPParser_NodeTraverser;
        $prettyPrinter = new PHPParser_PrettyPrinter_Default;

        $traverser->addVisitor($this);
        $n=0;
        foreach ($files as $file)
        {
            $n++;
            if (($n)%80==0) 
                echo PHP_EOL;
            else
                echo ".";
            try {
                $this->file=substr($file,strlen($path)+1);
                $syntax_tree = $parser->parse(file_get_contents($file));
                // $syntax_tree = $parser->parse('<?php $x=new \mysqli();');
            }
            catch (PHPParser_Error $e)
            {
                echo PHP_EOL."ERROR: Unable to parse {$file}: ".$e->getMessage().PHP_EOL;
                continue;
            }
            $filtered = $traverser->traverse($syntax_tree);
            // $newCode = '<?php ' .$comments.PHP_EOL.$prettyPrinter->prettyPrint($filtered);
        }
        echo PHP_EOL;
    }

    static private $sinkScore=10;
    function statistics()
    {
        if (empty($this->total))
            return "No data found.\n";
        echo "Generating statistics, please wait...".PHP_EOL;
        ob_start();
        $res=array();
        $total=array("count"=>0,"tracking"=>0,"score"=>0,"taint"=>0,"tracking"=>0,"inference"=>0,"sinks"=>0);
        foreach ($this->results as $file=>$data)
        {
            $tracking=0;
            $inference=0;
            if (isset($data['taint']))
            foreach ($data['taint'] as $taintFunction=>$count)
            {
                $t=self::$taintFunctions[$taintFunction];
                $tracking=$t['impact'] * $t['tracking'] * $count;
                $inference=$t['impact'] * $t['inference'] * $count;
            }
            $sinks=isset($data['sinks'])?array_sum($data['sinks']):0;
            $count=isset($data['taint'])?array_sum($data['taint']):0;
            $taint=$tracking+$inference;
            $res[$file]=array("file"=>$file,"score"=>$taint+$sinks*self::$sinkScore,"taint"=>$taint,
                "count"=>$count, "tracking"=>$tracking,"inference"=>$inference,'sinks'=>$sinks );

            foreach ($res[$file] as $k=>$v)
            {
                if ($k=="file") continue;
                $total[$k]+=$v;
            }
        }
        $tableHeader=sprintf("# %-57s %%  Score Sinks Funcs\n","File");
        $delimiter=str_repeat("-",80).PHP_EOL;
        echo $delimiter;
        echo "Most interesting files:".PHP_EOL;
        usort($res,function($t1,$t2){return $t1['score']<$t2['score'];});

        $totalShown=min(9,count($res)/9,count($res));
        echo $tableHeader;
        $fileLength=55;
        $n=0;
        for ($i=0;$i<$totalShown;++$i)
        {
            $t=$res[$i];
            $percentage=$t['score']/$total['score']*100;
            $f=$t['file'];
            if (strlen($f)>$fileLength)
            {
                $f=substr($f,0,strpos($f,"/"))."/...";
                $f.=substr($t['file'],-($fileLength-strlen($f)));
            }
            printf("%d %-{$fileLength}s %4.1f%% %4d %3d %4d\n",++$n,$f, $percentage,$t['score'],$t['sinks'],$t['count']);
        }

        echo PHP_EOL;
        echo $delimiter;
        echo "Most important sinks:".PHP_EOL;
        echo $tableHeader;
        $dup=$res;
        usort($dup,function($t1,$t2){return $t1['sinks']<$t2['sinks'];});
        $nn=0;
        for ($i=0;$i<min(3,count($dup));++$i)
        {
            $t=$dup[$i];
            if (!$t['sinks']) continue;
            $percentage=$t['score']/$total['score']*100;
            $f=$t['file'];
            if (strlen($f)>$fileLength)
            {
                $f=substr($f,0,strpos($f,"/"))."/...";
                $f.=substr($t['file'],-($fileLength-strlen($f)));
            }
            printf("%d %-{$fileLength}s %4.1f%% %4d %3d %4d\n",++$nn,$f, $percentage,$t['score'],$t['sinks'],$t['count']);
        }

        $report="";
        $report.= $delimiter;
        $report.= "Rest of the files:".PHP_EOL;
        $report.=$tableHeader;


        for ($i=$totalShown;$i<count($res);++$i)
        {
            $t=$res[$i];
            $percentage=$t['score']/$total['score']*100;
            $f=$t['file'];
            if (strlen($f)>$fileLength)
            {
                $f=substr($f,0,strpos($f,"/"))."/...";
                $f.=substr($t['file'],-($fileLength-strlen($f)));
            }
            $report.=sprintf("%".strlen(count($res))."d %-{$fileLength}s %4.1f%% %4d %3d %4d\n",++$n,$f, $percentage,$t['score'],$t['sinks'],$t['count']);
        }



        $this->report=$report;

        return ob_get_clean();
    }

    protected function likelihood($score,$files)
    {
        $avg=$score/(float)$files;
        if ($avg<1)
            $t="impossible";
        elseif ($avg<5)
            $t="highly unlikely";
        elseif ($avg<10)
            $t="unlikely";
        elseif ($avg<20)
            $t="somewhat unlikely";
        elseif ($avg<50)
            $t="possible";
        elseif ($avg<100)
            $t="likely";
        elseif ($avg<200)
            $t="very likely";
        else
            $t="definitely possible";



        return $t;
    }
}
class cmdainter implements AfterExpressionAnalysisInterface
{

    public static function afterExpressionAnalysis(
        PhpParser\Node\Expr $expr,
        Context $context,
        StatementsSource $statements_source,
        Codebase $codebase,
        array &$file_replacements = []
    ) { //psalm forward analysis
        global $current_node;
        if ($expr == $current_node) 
        {
            $expr_type = $statements_source->getNodeTypeProvider()->getType($expr);

            // should be a globally unique id
            // you can use its line number/start offset
            $expr_identifier = '$bad_data'. '-' . $statements_source->getFileName(). ':' . $expr->getAttribute('startFilePos');

            if ($expr_type) {
                $codebase->addTaintSource(
                    $expr_type,
                    $expr_identifier,
                    TaintKindGroup::ALL_INPUT,
                    new CodeLocation($statements_source, $expr)
                );
            }
            $forward_node = new forward_unit($expr,0);
            array_push($forward_tree,$forward_node);
            $var_analyzer = new variablee_Analyzer($forward_tree);
            $var_analyzer->process();
        }
    }
}

class string_extractor extends NodeVisitorAbstract
{

    public function leaveNode(Node $node)
    {
        global $cmd_list;
        global $dumper;
        global $current_arg;
        global $forward_tree;
        if($node instanceof String_)
        {
            $node_value = $node->value;
            
            $arg_list = explode(" ",$node_value);
            $result = in_array($arg_list[0],$cmd_list);
            if($result == True)
            {
                $current_arg = $node_value;
                $sink_result = in_sink_function($node);
                // echo gettype($sink_result)."\n";
                // if($arg_list[0]=="w")
                // {
                //     $parent_node = $node->getattribute("parent");
                //     echo $dumper->dump($parent_node) . "\n";
                //     $parent_node = $parent_node->getattribute("parent");
                //     echo $dumper->dump($parent_node) . "\n";
                //     $parent_node = $parent_node->getattribute("parent");
                //     echo $dumper->dump($parent_node) . "\n";

                // }
                if($sink_result!=False)
                {
                    $forward_node = new forward_unit($node,$sink_result);
                    array_push($forward_tree,$forward_node);
                    echo $arg_list[0]."\n";
                }                
            } 
        }
        elseif($node instanceof Encapsed)
        {
            $part = $node->parts[0];

            if($part instanceof EncapsedStringPart)
            {
                $arg_list = explode(" ",$part->value);
                $result = in_array($arg_list[0],$cmd_list);
                if($result == True)
                {
                    $current_arg = $part->value;
                    $sink_result = in_sink_function($part);
                    if($sink_result!=False)
                    {
                        $forward_node = new forward_unit($node,$sink_result);
                        array_push($forward_tree,$forward_node);
                        echo $arg_list[0]."\n";
                    }       
                } 
            }
            // foreach ($node->parts as $part)
            // {
            //     if($part instanceof EncapsedStringPart)
            //     {
            //         echo $part->value."\n";
            //     }
            // }
        }
    }
}

// $code = <<<'CODE'
// <?php

// function test($foo)
// {
//     var_dump($foo);
// }
// CODE;
$cmd_list = get_cmd_list();
$parser = (new ParserFactory)->create(ParserFactory::PREFER_PHP7);
$forward_traverser = new NodeTraverser();
// $traverser->addVisitor(new ParentConnectingVisitor);
$forward_traverser->addVisitor(new string_extractor);
$forward_traverser->addVisitor(new NodeConnectingVisitor);
// try {
//     $ast = $parser->parse($code);
// } catch (Error $error) {
//     echo "Parse error: {$error->getMessage()}\n";
//     return;
// }

$dumper = new NodeDumper;
// echo $dumper->dump($ast) . "\n";
$cmd_sink_functions = ["exec","passthru", "system", "popen", "shell_exec", "exec", "proc_open"];
$addionnal_trusted_func = ["ini_get"];
$files = getAllPhpFiles("/home/user/Desktop/plainview-activity-monitor-20161228");
// $files = getAllPhpFiles("/home/user/Desktop/html");
// var_dump($files);
$current_arg = "w";
$current_ast;
foreach ($files as $file)
{
	// if (($n)%80==0) 
	// 	echo PHP_EOL;
	// else
    //     echo ".";
    $syntax_tree = $parser->parse(file_get_contents($file));
    $current_ast = $syntax_tree;
    $filtered = $forward_traverser->traverse($syntax_tree);

}
$tmp_result = bi_analysis($forward_tree,$backward_tree);
?>