<?php
require("../config_loader.php");
$CLASS_TYPE = ["0"=>"class","16"=>"abs_class","interface"];
use PhpParser\{Node, NodeFinder};
use PhpParser\NodeTraverser;
use PhpParser\NodeVisitor\ParentConnectingVisitor;
use PhpParser\ParserFactory;
use PhpParser\NodeVisitor\NameResolver;
use PhpParser\NodeVisitor\NodeConnectingVisitor;
$magicNames = ['__construct','__destruct','__call','__callstatic','__get','__set','__isset','__unset','__sleep','__wakeup','__tostring','__set_state','__clone','__invoke','__debuginfo',];
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
function test(){
    $prettyPrinter = new PhpParser\PrettyPrinter\Standard;
    $test_file = file_get_contents("../test/class_test.php");
    $ast = analyze_general($test_file);
    $classes = collect_class_info($ast);
    $results = [];
    foreach($classes as $key=>$class)
    {
        $class_result = analyze_class($class);
        var_dump($class_result->name);
        var_dump($class_result->parentclass);
        print("------------------------\n");
        $results[] = $class_result;
    }
    var_dump($results);
}
test();
?>