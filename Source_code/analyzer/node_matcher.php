<?php
$forward_record;
$backward_record;
function node_match($forward_node,$backward_node)
{
    global $search_record;
    $cur_forward = $forward_node;
    $cur_backward = $forward_node;
    while($cur_forward!=0)
    {
        while($cur_backward != 0)
        {
            if($cur_backward == $cur_forward)
            {
                return 1;
            }
            else
            {
                if($cur_backward->adjacen_node!= 0)
                {
                    $cur_backward = $cur_backward->adjacent_node;
                    $result = node_match($cur_forward,$cur_backward);
                    return $result;
                }
                elseif($cur_backward->backward_node!= 0)
                {
                    $cur_backward = $cur_backward->backward_node;
                    $result = node_match($cur_forward,$cur_backward);
                    return $result;
                }
                else
                {
                    return 0;
                }
                
            }
        }
        $cur_forward = $cur_forward ->forward_node;
        $result = node_match($cur_forward,$cur_backward);
        return $result;
    }
    return 0;
}
function bi_analysis($forward_list,$backward_list)
{
    global $forward_record;
    global $backward_record;
    foreach($forward_list as $forward_node)
    {
        foreach($backward_list as $backward_node)
        {
            $result = node_match($forward_node,$backward_node);
            if($result == 1)
            {
                array_push($forward_record,$forward_node);
                array_push($backward_record,$backward_node);
            }
        }
    }
}
?>