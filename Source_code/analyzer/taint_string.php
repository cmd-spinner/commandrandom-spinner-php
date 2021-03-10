<?php

namespace Some\Ns;

use PhpParser;
use Psalm\Codebase;
use Psalm\CodeLocation;
use Psalm\Context;
use Psalm\FileManipulation;
use Psalm\Plugin\Hook\AfterExpressionAnalysisInterface;
use Psalm\StatementsSource;
use Psalm\Type\TaintKindGroup;
use PhpParser\Node\Scalar\EncapsedStringPart;
use PhpParser\Node\Scalar\Encapsed;
$cmd_list = get_cmd_list();
class const_string_Tainter implements AfterExpressionAnalysisInterface
{
    public static function afterExpressionAnalysis($expr,$context,$statements_source,$codebase,array &$file_replacements = []) {
        global $cmd_list;
        if ($expr instanceof PhpParser\Node\Scalar\String_ || $expr instanceof PhpParser\Node\Scalar\EncapsedStringPart || $expr instanceof PhpParser\Node\Scalar\Encapsed) 
        {
            if($expr instanceof PhpParser\Node\Scalar\String_)
            {
                $node_value = $expr->value;
                $arg_list = explode(" ",$node_value);
                $result = in_array($arg_list[0],$cmd_list);
                if($result == false)
                {
                    return 0;
                }
                $sink_result = in_sink_function($expr);
                if($sink_result!=False)
                {
                    var_dump($sink_result);
                    return 0;
                }   
            }
            if($expr instanceof PhpParser\Node\Scalar\Encapsed)
            {
                $part = $expr->parts[0];
                if($part instanceof EncapsedStringPart)
                {
                    $arg_list = explode(" ",$part->value);
                    $result = in_array($arg_list[0],$cmd_list);
                    if($result == false)
                    {
                        return 0;
                    }
                    if($result == True)
                    {
                        $current_arg = $part->value;
                        $sink_result = in_sink_function($part);
                        var_dump($sink_result);
                        return 0;   
                    } 
                }
            }
            $expr_type = $statements_source->getNodeTypeProvider()->getType($expr);

            // should be a globally unique id
            // you can use its line number/start offset
            $expr_identifier = 'str'
                . '-' . $statements_source->getFileName()
                . ':' . $expr->getAttribute('startFilePos');

            if ($expr_type) {
                $codebase->addTaintSource(
                    $expr_type,
                    $expr_identifier,
                    TaintKindGroup::ALL_INPUT,
                    new CodeLocation($statements_source, $expr)
                );
            }
        }
    }
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