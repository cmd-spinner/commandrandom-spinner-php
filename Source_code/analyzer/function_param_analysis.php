<?
$FUNCTION_TYPE = ["file","class"];
class function_instance{
    public $function_name;
    public $args = [];
    public $args_type = [];
    public $return;
    public $call = [];
    public $call_by = [];
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