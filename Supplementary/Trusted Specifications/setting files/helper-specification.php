<?php

use PhpParser\{Node, NodeTraverser, NodeVisitorAbstract,NodeFinder};
use PhpParser\Error;
use PhpParser\NodeDumper;
use PhpParser\ParserFactory;
use PhpParser\Node\Expr\FuncCall;
use PhpParser\Node\Scalar\String_;
use PhpParser\Node\Expr\Variable;
use analyzer\phpanalyzer\taint;
require("../config_loader.php");


use PhpParser\NodeVisitor\ParentConnectingVisitor;
use PhpParser\NodeVisitor\NameResolver;
use PhpParser\NodeVisitor\NodeConnectingVisitor;


$shell_sinks = ["passthru", "system", "popen", "shell_exec","exec","proc_open"];
$db_sinks = ["multi_query","prepare","mysql_query","mysql_real_query","sqlite_query","sqlite_exec","real_query","send_query","sqlite_array_query","sqlite_single_query","sqlite_unbuffered_query"];
$api_getenv = "getenv";
$api_setenv = "setenv";
$api_config = ["init_get","ini_get_all","parse_ini_file"];

// $trusted_dir = ["/var/www/"];//input
$trusted_dir = [];

$oop_poly_arr=[];
$class_node=[];
function analyze_general($file_content)
{
    $traverser = new NodeTraverser;
    $nameResolver = new PhpParser\NodeVisitor\NameResolver;

    // $traverser->addVisitor(new ParentConnectingVisitor);//get sibling node
    // $traverser->addVisitor(new NodeConnectingVisitor);//get parent node
    // $nodeTraverser->addVisitor($nameResolver);//in case we need to resolve namespace 
    $parser = (new ParserFactory)->create(ParserFactory::PREFER_PHP7);
    $ast = $parser->parse($file_content);
    $ast = $traverser->traverse($ast);
    return $ast;
}
$CLASS_TYPE = ["0"=>"class","16"=>"abs_class","interface"];
$magicNames = ['__construct','__destruct','__call','__callstatic','__get','__set','__isset','__unset','__sleep','__wakeup','__tostring','__set_state','__clone','__invoke','__debuginfo',];
class Class_Instance{
    public $name;
    public $subclass = [];
    public $parentclass = [];
    public $implements = [];
    public $type;
    public $methods = [];
    public $properties = [];
    public $flag;
    public $node;
    public function __construct($name,$subclass = NULL,$parentclass,$implements,$type,$methods,$properties,$flag,$node)
    {
        $this->name = $name;
        $this->subclass = $subclass;
        $this->parentclass = $parentclass;
        $this->implements = $implements;
        $this->type = $type;
        $this->methods = $methods;
        $this->properties = $properties;
        $this->flag = $flag;
        $this->node = $node;
    }
}
class Class_Property{
    public $name;
    public $attribute;
    public $node;
    public function __construct($name,$attr,$node){
        $this->name = $name;
        $this->attribute = $attr;
        $this->node = $node;
    }
}
class Class_Method{
    public $name;
    public $attribute;//public, private or protected
    public $args = [];// store variable name and default value
    public $params;//original params ast
    public $ret;// ret's ast 
    public $body;
    public $node;
    public function __construct($name,$attr,$args,$params,$ret,$body,$node){
        $this->name = $name;
        $this->attribute = $attr;
        $this->args = $args;
        $this->params = $params;
        $this->ret = $ret;
        $this->body = $body;
        $this->node = $node;
        
    }
}
function collect_class_info($ast)
{
    $nodeFinder = new NodeFinder;
    $classes = $nodeFinder->findInstanceOf($ast, Node\Stmt\Class_::class);
    return $classes;
}

function analyze_class($class)
{
    $class_name = $class->name->name;//get class name
    if(!is_null($class->extends))
    {
        $parent_class = $class->extends->parts[0];//get parent class

    }
    else{
        $parent_class = NULL;
    }
    if(!is_null($class->implements))
    {
        $implements=[];
        foreach($class->implements as $num=>$value)
        {
            $implement=$value->parts[0];
            $implements[] = $implement;//get implemented class
        }
    }
    else{
        $parent_class = NULL;
    }
    //get class type
    if($class->flags == 0)
    {
        $class_type = "class";
    }
    else if($class->flags == 16)
    {
        $class_type = "abstract_class";
    }
    else{
        $class_type = "unknown";
    }
    
    //get class properties
    $class_properties = analyze_class_properties($class);
    // get class methods
    $class_methods = analyze_class_methods($class);
    $class_instance = new Class_Instance($class_name,NULL,$parent_class,$implements,$class_type,$class_methods,$class_properties,$class->flags,$class);
    return $class_instance;
}
function analyze_class_properties($class)
{
    $properties = $class->getProperties();
    $class_properties = [];
    foreach ($properties as $property)
    {
        $prop = $property->props;
        $flag = $property->flags;
        $prop_name = $prop[0]->name->name;
        if($flag == 4){
            $prop_attribute = "private";
        }
        else if($flag == 1){
            $prop_attribute = "public";
        }
        else if($flag == 13){
            $prop_attribute = "protected";
        }
        else{
            $prop_attribute = "unknown";
        }
        $class_property = new Class_Property($prop_name,$prop_attribute,$property);
        $class_properties[] = $class_property;
        return $class_properties;
    }
}
function analyze_class_methods($class)
{
    $nodeFinder = new NodeFinder;
    $methods = $class->getMethods();
    $class_methods = [];
    foreach($methods as $method)
    {
        $flag = $method->flags;
        $name = $method->name->name;
        $ret_type = $method->returnType;
        $params = $method->params;
        $statements = $method->getStmts();
        $ret = $nodeFinder->find($statements, function(Node $node) {return $node instanceof Node\Stmt\Return_;});
        $args = analyze_params($params);
        if($flag == 4){
            $attr = "private";
        }
        else if($flag == 1){
            $attr = "public";
        }
        else if($flag == 13){
            $attr = "protected";
        }
        else{
            $attr = "unknown";
        }
        $class_method = new Class_Method($name,$attr,$args,$params,$ret,$statements,$method);
        $class_methods[] = $class_method;
    }
    return $class_methods;
}

function analyze_params($params)
{
    if(count($params)==0)
    {
        return NULL;
    }
    $param_arr = array();
    foreach($params as $param){
        $var_name = $param->var->name;
        if(is_null($param->default->value))
        {
            $var_default_value = NULL;
        }
        else
        {
            $var_default_value = $param->default->value;
        }
        $tmp_arr = array($var_name=>$var_default_value);
        $param_arr = array_merge($param_arr,$tmp_arr);
    }
    return $param_arr;
}

function analyze_interface($interfaces){


}
function check_oop_poly($class_instances){
    // $class_method_arr = [];
    foreach($class_instances as $key=>$class_instance){
        // $method_arr = [];
        // foreach($class_intance->methods as $method){
        //     $method_arr[]=$method->name;
        // }
        $parentclassname = $class_instance->parentclass;
        foreach($class_instances as $class){
            if($class->name == $parentclassname){
                $class->subclass[] = $class_instance;
            }
        }
        // $class_method_arr=array_merge($class_method_arr,array($class_instance=>$method_arr));
    }//get subclass info from parent class


}

function check_method_exist_by_name($class,$method_name){
    foreach($class->methods as $method){
        if($method->name == $method_name){
            return true;
        }
    }
    return false;
}

//test code
$code = <<<'CODE'
<?php
function printTips() {
    global $tips;
    foreach($tips as $i => $tip) {
        echo "Tip $i: " . $tip;
    }
  system("ls");
}
function test($foo)
{
    $a = "ED";
    $newed = getenv($a);
    $ed = getenv("EDITOR");
    $ed = getenv("SHELL");
    var_dump($foo);
}
CODE;

$parser = (new ParserFactory)->create(ParserFactory::PREFER_PHP7);
try {
    $ast = $parser->parse($code);
} catch (Error $error) {
    echo "Parse error: {$error->getMessage()}\n";
    return;
}

$dumper = new NodeDumper;
//test code end



$flag_shell = 0;
$flag_db = 0;
$flag_getenv = 0;
$flag_config = 0;
$flag_wp = 0;
$flag_setenv = 0;
$flag_php_ini = 0;


$flag_const = 0;
$flag_config = 0;
$flag_env = 0;
$flag_folder = 0;

$func_call_src =[];
function get_bin_list()
{
    $file_names = [];
    $folders = getenv("PATH");
    $folders = explode(":",$folders);
    // var_dump($folders);
    foreach($folders as $folder)
    {
        $files = array_diff(scandir($folder),array(".",".."));
        $file_names = array_merge($files,$file_names);
    }
    
    return $file_names;
}

$names = get_bin_list();


function get_const_str($stmts){
    $nodeFinder = new NodeFinder;
    $full_consts = $nodeFinder->findInstanceOf($stmts,Node\Scalar\String_::class);
    return $full_consts;
}

//Node\Expr\Expr_FuncCall
function get_func_list($stmts){
    $nodeFinder = new NodeFinder;
    $func_calls = $nodeFinder->findInstanceOf($stmts,Node\Expr\FuncCall::class);
    return $func_calls;
}

$func_calls = get_func_list($ast);
foreach ($func_calls as $index=>$func_call){
    array_push($func_call_src,$func_call->name->parts[0]);
}

$result = [];

foreach($func_call_src as $index=>$funcName){
    if(in_array($funcName,$shell_sinks)){
        $flag_shell = 1;
    }
    if(in_array($funcName,$db_sinks)){
        $flag_db = 1;
    }
    if($funcName== $api_getenv){
        $flag_getenv = 1;
    }
    if($funcName== $api_setenv){
        $flag_setenv = 1;
    }
    if(in_array($funcName,$api_config)){
        $flag_config = 1;
    }
}
function forward_taint_test($ast,$node){
    return true;
}

function eval_getenv($node){
    global $names;
    global $flag_env;
    $arg = $node->args[0];
    // var_dump($arg);
    if($arg->value instanceof PhpParser\Node\Expr\Variable){
        return 0;
    }
    if($arg->value instanceof PhpParser\Node\Scalar\String_){
        $env_var = getenv($arg->value->value);
        // var_dump($env_var);
        if(in_array(basename($env_var),$names)){
            $flag_env = 1;
            return 1;
        }
        else{
            return 0;
        }
    }
}


function eval_iniget($node){
    global $names;
    global $flag_php_ini;
    $arg = $node->args[0];
    // var_dump($arg);
    if($node->name->parts[0] == "ini_get"){
        if($arg->value instanceof PhpParser\Node\Expr\Variable){
            return 0;
        }
        if($arg->value instanceof PhpParser\Node\Scalar\String_){
            $ini_var = ini_get($arg->value->value);
            if(in_array(basename($ini_var),$names)){
                $flag_php_ini = 1;
    
                return 1;
            }
            else{
                return 0;
            }
        }
    }

}

$consts = get_const_str($ast);
$bins = get_bin_list();

foreach($consts as $index => $const){
    if(in_array(basename($const->value),$bins)){
        if(forward_taint_test($ast,$const)){
            $flag_const = 1;
            array_push($result,"constant");
            break;
        }
    }
}

// var_dump($flag_const);

if($flag_getenv == 1){
    foreach($func_calls as $index=>$func_call){
        if($api_getenv == $func_call->name->parts[0]){
            $tmp_flag = eval_getenv($func_call);
            if(forward_taint_test($ast,$const) and $tmp_flag == 1){
                $flag_env = $tmp_flag;
                array_push($result,"getenv");
            }
            // var_dump($tmp_flag);
        }
    }
}

if($flag_config == 1){
    foreach($func_calls as $index=>$func_call){
        if(in_array($func_call->name->parts[0],$api_config)){
            $tmp_flag = eval_iniget($func_call);
            if(forward_taint_test($ast,$const)){
                $flag_env = $tmp_flag;
                $ini_file = php_ini_loaded_file();
                array_push($result,$ini_file);
            }
            // var_dump($tmp_flag);
        }
    }
}

if(!empty($trusted_dir)){
    foreach ($trusted_dir as $index => $dir){
        array_push($result,$dir);
    }
}
var_dump($result);
?>