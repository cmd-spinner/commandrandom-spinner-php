<?php

include_once(dirname(__FILE__) . '/../CallGraph/CallGraph.php');
include_once(dirname(__FILE__) . '/../CallGraph/CallGraphNode.php');
include_once(dirname(__FILE__) . '/../CFG/CFGNode.php');
include_once(dirname(__FILE__) . '/../CFG/CFGNodeCond.php');
include_once(dirname(__FILE__) . '/../CFG/CFGNodeStmt.php');
include_once(dirname(__FILE__) . '/../PHP-Parser-master/lib/bootstrap.php');
include_once(dirname(__FILE__) . '/../CFG/StmtProcessing.php');
include_once('TaintedVariables.php');
include_once('CFGTaintMap.php');
include_once('FileTaintMap.php');
include_once('TaintMap.php');
include_once('TaintSource.php');

// Checks whether a conditional node is tainted.
function isSecretTaintedCFGNodeCond($current_node, $taint_set) {

	 // If the conditional contains an assignment, propagate its taint
         // besides checking for taint in the conditional.
	 if ($current_node->expr instanceof PhpParser\Node\Expr\Assign) {

	    	return isTainted($current_node->expr->expr, $tainted_set, False);
         }
         else {
	       	        
		return isTainted($current_node->expr, $taint_set, False);
         }
}

// TODO: Change hardwired notions of taint for a specific application.
// Checks whether an expression is tainted, by checking whether a parameter is a tainted variable or a user/secret input. The $user_taint parameter is True when checking for user taint, and 
// false when checking for secret taint.
function isTainted($expr, $tainted_variables, $user_taint) {

       print "Analyzing expression for taint.\n";
       //print "The class is " . get_class($expr) . "\n";

       if ($expr == null) {
       
	return False;
       }

       // Check if it's a pre-defined source of taint.
       if ($user_taint && TaintSource::isUserTaintSource($expr)) {

       	  return True;
       }
       else if (!$user_taint && TaintSource::isSecretTaintSource($expr)) {

       	  return True;
       }

       // For now, checking that the expression is either a function call of 'postGetSession' or a variable already in the tainted set.

       if ($expr instanceof PhpParser\Node\Expr\StaticCall || $expr instanceof PhpParser\Node\Expr\FuncCall || $expr instanceof PhpParser\Node\Expr\MethodCall) {

       	  print "Analyzing static call, function call or method call for taint\n";       	  
	  $function_name = $expr->name;

	  // The expression is tainted if it invokes a basic user input extraction function.
	  if ($user_taint && (strcmp($function_name, 'postGetSessionInt') == 0 || strcmp($function_name, 'postGetSessionString') == 0)) {

	     return true;
	  }
	  else if (!$user_taint && strcmp($function_name, 'search') == 0) {

	     // The expression is tainted if it invokes the secret-tainting function in openclinic.
	     return true;
	  }
	  else if (!$user_taint && strcmp($function_name, 'mysql_query') == 0) {
	     
	     // TODO: Fix this to only taint select statements.
	     // The expression is tainted if it invokes the mysql_query predefined function.
	     return true;
	  }

	  // The expression is tainted if one of the arguments is tainted.
	  foreach ($expr->args as $arg) {
	  	  
		  if (isTainted($arg->value, $tainted_variables, $user_taint)) {

		  	return true;
		  }
	  }
	  
	  // The expression is tainted if it is a method call over a tainted expression.
	  if ($expr instanceof PhpParser\Node\Expr\MethodCall && isTainted($expr->var, $tainted_variables, $user_taint)) {

	     return true;
	  }
       }
       else if ($expr instanceof PhpParser\Node\Expr\Variable) {

       	  print "Analyzing variable for taint : " . ($expr->name) . "\n";
       
	  return $tainted_variables->contains($expr->name);
       }
       else if ($expr instanceof PhpParser\Node\Expr\BinaryOp) {

       	  print "Analyzing binary op for taint.\n";
	  return isTainted($expr->left, $tainted_variables, $user_taint) || 
	  	 isTainted($expr->right, $tainted_variables, $user_taint);
       }
       else if ($expr instanceof PhpParser\Node\Expr\ArrayDimFetch) {

       	  print "Analyzing array fetch expression for taint.\n";

	  // The expression is user tainted if it invokes a basic predefined extraction
	  // from the '_GET' or '_POST' arrays.
	  if ($user_taint && (strcmp($expr->var->name, '_GET') == 0 || strcmp($expr->var->name, '_POST') == 0)) {

	     return true;
	  }
  
	  return isTainted($expr->var, $tainted_variables, $user_taint) || isTainted($expr->dim, $tainted_variables, $user_taint);
       }
       
       return false;
}

// Processes taint for a CFG node.
function processTaint($current_node, $user_tainted_variables_map, $secret_tainted_variables_map, $cfg_taint_map) {

	       // Check if the current node is a statement node with a 
	       // non-null statement.
	       if (CFGNode::isCFGNodeStmt($current_node) && $current_node->stmt) {

	       	  $stmt = $current_node->stmt;
	       	  // Check to see if the statement is an assigment,
		  // and the right hand side is tainted.
		  if (($stmt instanceof PhpParser\Node\Expr\Assign) || ($stmt instanceof PhpParser\Node\Expr\AssignOp)) {

		      // Accounting for simple LHS variables and array indexing.
		      $lhs = $stmt->var;
		      
		      if ($lhs instanceof PhpParser\Node\Expr\Variable) {
		      	 
			 $lhs_var = $lhs->name;
		      } 
		      else if ($lhs instanceof PhpParser\Node\Expr\ArrayDimFetch) {

		      	 $lhs_var = $lhs->var->name;
		      } else {

		      	 $lhs_var = null;
		      	 print "ERROR: Unrecognized LHS type of an assignment while performing taint analysis.\n";
		      } 
		     
		      if ($lhs_var && !$user_tainted_variables_map[$current_node]->contains($lhs_var)
		          && isTainted($stmt->expr, $user_tainted_variables_map[$current_node], True)) {

		      	 $user_tainted_variables_map[$current_node]->attach($lhs_var);
		     	 print "The variable " . ($lhs_var) . " became user-tainted.\n";
		      }

		      if ($lhs_var && !$secret_tainted_variables_map[$current_node]->contains($lhs_var)
		          && isTainted($stmt->expr, $secret_tainted_variables_map[$current_node], False)) {

		      	 $secret_tainted_variables_map[$current_node]->attach($lhs_var);
		     	 print "The variable " . ($lhs_var) . " became secret-tainted.\n";
		      }
		  }
		  // or a return statement.
		  else if($stmt instanceof PhpParser\Node\Stmt\Return_) {

		      // If the expression of the return statement is tainted, we label the CFG
		      // as producing a taint return value.
		      if (isTainted($stmt->expr, $user_tainted_variables_map[$current_node], True)) {

		     	 print "The return statement is user-tainted.\n";
			 $cfgTaintMap->setReturnsUserTaint(True);
		      }

		      if (isTainted($stmt->expr, $secret_tainted_variables_map[$current_node], False)) {

		     	 print "The return statement is secret-tainted.\n";
			 $cfgTaintMap->setReturnsSecretTaint(True);
		      }
		  }
	       }
	       // Check if a conditional node is tainted, and issue a warning.
	       else if (CFGNode::isCFGNodeCond($current_node) && $current_node->expr) {

	       	    // If the conditional contains an assignment, propagate its taint
		    // besides checking for taint in the conditional.
		    if ($current_node->expr instanceof PhpParser\Node\Expr\Assign) {

		       if (!$user_tainted_variables_map[$current_node]->contains($current_node->expr->var->name)
		           && isTainted($current_node->expr->expr, $user_tainted_variables_map[$current_node], True)) {
		       	  
			  print "The variable " . ($current_node->expr->var->name) . "became user tainted.\n";
			  $user_tainted_variables_map[$current_node]->attach($current_node->expr->var->name);
	       	    	  print "WARNING: Conditional node is user-tainted:\n";
		       }

		       if (!$secret_tainted_variables_map[$current_node]->contains($current_node->expr->var->name)
		           && isTainted($current_node->expr->expr, $secret_tainted_variables_map[$current_node], False)) {
		       	  
			  print "The variable " . ($current_node->expr->var->name) . "became secret tainted.\n";
			  $secret_tainted_variables_map[$current_node]->attach($current_node->expr->var->name);
	       	    	  print "WARNING: Conditional node is secret-tainted:\n";
		       }
		    }
		    else {
	       	        
			if (isTainted($current_node->expr, $secret_tainted_variables_map[$current_node], False)) {

	       	    	   print "WARNING: Conditional node is secret-tainted:\n";
			}

			if (isTainted($current_node->expr, $user_tainted_variables_map[$current_node], True)) {

	       	    	   print "WARNING: Conditional node is user-tainted:\n";
			}
		    }
	       }
	       // Check if a loop header is secret-tainted, and issue a warning.
	       else if (CFGNode::isCFGNodeLoopHeader($current_node) && $current_node->expr) {

	            print "Analyzing loop header.\n";
	            // The conditional covers the case when the condition is a boolean expression or an 
		    // assignment that propagates taint.
	       	    if ($current_node->isWhileLoop()) {

		       // Propagate taint when the conditional consists of an assignment.
		       if ($current_node->expr->cond instanceof PhpParser\Node\Expr\Assign) {

		       	  if (!$user_tainted_variables_map[$current_node]->contains($current_node->expr->cond->var->name)
		              && isTainted($current_node->expr->cond->expr, $user_tainted_variables_map[$current_node], True)) {
		       	  
				print "The variable " . ($current_node->expr->cond->var->name) . "became user tainted.\n";
			  	$user_tainted_variables_map[$current_node]->attach($current_node->expr->cond->var->name);
	       	    	  	print "WARNING: Loop header node is user-tainted:\n";
		       	  }

		       	  if (!$secret_tainted_variables_map[$current_node]->contains($current_node->expr->cond->var->name)
		              && isTainted($current_node->expr->cond->expr, $secret_tainted_variables_map[$current_node], False)) {
		       	  
				print "The variable " . ($current_node->expr->cond->var->name) . "became secret tainted.\n";
			  	$secret_tainted_variables_map[$current_node]->attach($current_node->expr->cond->var->name);
	       	    	  	print "WARNING: Loop header node is secret-tainted:\n";
		       	  }
		       }
		       else {

		       	    if (isTainted($current_node->expr->cond, $user_tainted_variables_map[$current_node], True)) {
		       
					print "While Loop is user-tainted.\n";
		       	    }

		       	    if (isTainted($current_node->expr->cond, $secret_tainted_variables_map[$current_node], False)) {
		       
					print "While Loop is secret-tainted.\n";
		       	    }
	       	       }
	       	     }
		     else if ($current_node->isForLoop()) {
		     	  
			  // Detect taint for conditional expressions of the for loop.
			  foreach ($current_node->expr->cond as $condExpr) {
			  
				if (isTainted($condExpr, $user_tainted_variables_map[$current_node], True)) {
		       
					print "For Loop condition is user-tainted.\n";
		       	        }

		       	        if (isTainted($condExpr, $secret_tainted_variables_map[$current_node], False)) {
		       
					print "For Loop is secret-tainted.\n";
		       	        }	  
			  }
		     }
		     else if ($current_node->isForeachLoop()) {
		     	  
			  // Detect taint for source expression of the foreach loop.
			  foreach ($current_node->expr->expr as $sourceExpr) {
			  
				if (isTainted($sourceExpr, $user_tainted_variables_map[$current_node], True)) {
		       
					print "Foreach Loop condition is user-tainted.\n";
		       	        }

		       	        if (isTainted($sourceExpr, $secret_tainted_variables_map[$current_node], False)) {
		       
					print "Foreach Loop is secret-tainted.\n";
		       	        }	  
			  }
		     }
	      }
}

// Performs a taint analysis across the entire program.
function taintAnalysis($callGraph, $cfgInfoMap, $functionSignatures) {

	 // Global map of taint analysis results.
	 $taintMap = new TaintMap();

	 // Create a queue of call graph nodes of the functions to analyze.
	 $callGraphNodeQueue = new SplQueue();

	 // Create a set of call graph nodes currently in the queue,
	 // to prevent nodes from being added multiple times.
	 $callGraphNodeSet = new SplObjectStorage();

	 // Initially, add all the call graph leaves.
	 foreach($callGraph->getLeaves() as $callGraphNode) {
	     $callGraphNodeQueue->enqueue($callGraphNode);
	     $callGraphNodeSet->attach($callGraphNode);
	 }

	 // Process the nodes while the queue is not empty.
	 while(!$callGraphNodeQueue->isEmpty()) {

	     $callGraphNode = $callGraphNodeQueue->dequeue();
	     //$callGraphNodeSet->detach($callGraphNode);

	     $signature = $callGraphNode->getFunctionRepresentation();
	     $fileName = $signature->getFileName();

	     // Process the CFG of a function if it's user defined.
	     if($fileName && isset($cfgInfoMap[$fileName])) {
	         $fileCFGInfo = $cfgInfoMap[$fileName];
	         $cfg = $fileCFGInfo->getCFG($signature);
		 print "Starting taint analysis on function with signature: " .
                       ($signature->toString()) . "\n";
	         $cfgTaintMap = cfgTaintAnalysis($cfg, $signature, $cfgInfoMap, $functionSignatures);
		 print "Finished taint analysis on function with signature: " .
                       ($signature->toString()) . "\n";
		 $taintMap->put($signature->toString(), $cfgTaintMap);
	     }

	     // Add the predecessors in the call graph, if they're not already
	     // present in the queue.
	     foreach($callGraphNode->getPredecessors() as $predecessor) {
	            if(!$callGraphNodeSet->contains($predecessor)) {
		    	    
		        $callGraphNodeSet->attach($predecessor);
			$callGraphNodeQueue->enqueue($predecessor);	        
		    }
	     }
	 }

	 return $taintMap;
}


// Performs a flow-sensitive forward taint analysis on the defined functions
// and the main code.
function fileTaintAnalysis($fileCFGInfo) {
	 
	 $mainCFG = $fileCFGInfo->getMainCFG();
	 $functionCFGs = $fileCFGInfo->getFunctionCFGs();
	 $functionSignatures = $fileCFGInfo->getFunctionSignatures();
	 
	 // Initialize pre-defined taint information.
	 TaintSource::initializeTaintSources();

	 // Construction the taint map for the main function.
	 $mainTaintMap = cfgTaintAnalysis($mainCFG);


	 // Constructing the taint maps for each internal function.	 
	 $functionTaintMaps = array();

	 foreach ($functionCFGs as $functionName => $functionCFG) {

	 	 $functionTaintMap = cfgTaintAnalysis($functionCFG);
		 $functionTaintMaps[$functionName] = $functionTaintMap;
	 }

	 return new FileTaintMap($mainTaintMap, $functionTaintMaps);
}

// Performs a flow-sensitive forward taint analysis on a CFG.
function cfgTaintAnalysis($cfg, $signature, $cfgInfoMap, $functionSignatures) {

	 print "Starting Taint Analysis.\n";

	 // WARNING: Imposing a bound to avoid infinite loops.
	 $steps = 0;
	 $bound = 10000;

	 // Map that contains the set of tainted variables 
	 // per CFG node.
	 $user_tainted_variables_map = new SplObjectStorage();
	 $secret_tainted_variables_map = new SplObjectStorage();

	 $cfg_taint_map = new CFGTaintMap($user_tainted_variables_map, $secret_tainted_variables_map);

	 // Forward flow-sensitive taint-analysis.
	 $entry_node = $cfg->entry;
	 $q = new SplQueue();
	 $q->enqueue($entry_node);

	 while (count($q) && $steps < $bound) {
	       
	       $current_node = $q->dequeue();

	       $steps++;

	       if (!$user_tainted_variables_map->contains($current_node)) {

	       	  $user_tainted_variables_map[$current_node] = new TaintedVariables();
	       }

	       if (!$secret_tainted_variables_map->contains($current_node)) {

	       	  $secret_tainted_variables_map[$current_node] = new TaintedVariables();
	       }

	       print "Started processing node: \n";
	       $current_node->printCFGNode();

	       $initial_user_tainted_size = $user_tainted_variables_map[$current_node]->count();
	       $initial_secret_tainted_size = $secret_tainted_variables_map[$current_node]->count();

	       // Add the taint sets of the parents.
	       foreach($current_node->parents as $parent) {
	       		
			if ($user_tainted_variables_map->contains($parent)) {

			   $user_tainted_variables_map[$current_node]->addAll($user_tainted_variables_map[$parent]);
			}

			if ($secret_tainted_variables_map->contains($parent)) {

			   $secret_tainted_variables_map[$current_node]->addAll($secret_tainted_variables_map[$parent]);
			}
	       }

	       // Process taint for the current node.
	       processTaint($current_node, $user_tainted_variables_map, $secret_tainted_variables_map, $cfg_taint_map);

	       $changed = $initial_user_tainted_size != $user_tainted_variables_map[$current_node]->count() 
	       		  || $initial_secret_tainted_size != $secret_tainted_variables_map[$current_node]->count() ;

	       print "Finished processing node: \n";
	       $current_node->printCFGNode();

	       print "User tainted variables:\n";
	       $user_tainted_variables_map[$current_node]->printTaintedVariables();
	       print "Secret tainted variables:\n";
	       $secret_tainted_variables_map[$current_node]->printTaintedVariables();
	       print "\n";

	       // Add the successors of the current node to the queue, if the tainted set has changed or the successor hasn't been visited.

	       foreach ($current_node->successors as $successor) {

	       	       if ($changed || !$user_tainted_variables_map->contains($successor) 
		                    || !$secret_tainted_variables_map->contains($successor)) {

			      $q->enqueue($successor);
		       }
	       }
	}

	print "==============================\n";
	print "The user tainted variables at the exit node are:\n";
	$user_tainted_variables_map[$cfg->exit]->printTaintedVariables();
	print "\n";
	print "==============================\n";
	print "==============================\n";
	print "The secret tainted variables at the exit node are:\n";
	$secret_tainted_variables_map[$cfg->exit]->printTaintedVariables();
	print "\n";
	print "==============================\n";
	
	return $cfg_taint_map;
}
?>