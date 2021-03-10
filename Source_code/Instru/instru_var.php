<?php
use PhpParser\{Node, NodeTraverser, NodeVisitorAbstract};
use PhpParser\BuilderFactory;
use PhpParser\PrettyPrinter;
use rand\random_node;
$factory = new BuilderFactory;

//test

$node = $factory->namespace('Name\Space')
    ->addStmt($factory->use('Some\Other\Thingy')->as('SomeClass'))
    ->addStmt($factory->useFunction('strlen'))
    ->addStmt($factory->useConst('PHP_VERSION'))
    ->addStmt($factory->class('SomeOtherClass')
        ->extend('SomeClass')
        ->implement('A\Few', '\Interfaces')
        ->makeAbstract() // ->makeFinal()

        ->addStmt($factory->useTrait('FirstTrait'))

        ->addStmt($factory->useTrait('SecondTrait', 'ThirdTrait')
            ->and('AnotherTrait')
            ->with($factory->traitUseAdaptation('foo')->as('bar'))
            ->with($factory->traitUseAdaptation('AnotherTrait', 'baz')->as('test'))
            ->with($factory->traitUseAdaptation('AnotherTrait', 'func')->insteadof('SecondTrait')))

        ->addStmt($factory->method('someMethod')
            ->makePublic()
            ->makeAbstract() // ->makeFinal()
            ->setReturnType('bool') // ->makeReturnByRef()
            ->addParam($factory->param('someParam')->setType('SomeClass'))
            ->setDocComment('/**
                              * This method does something.
                              *
                              * @param SomeClass And takes a parameter
                              */')
        )

        ->addStmt($factory->method('anotherMethod')
            ->makeProtected() // ->makePublic() [default], ->makePrivate()
            ->addParam($factory->param('someParam')->setDefault('test'))
            // it is possible to add manually created nodes
            ->addStmt(new Node\Expr\Print_(new Node\Expr\Variable('someParam')))
        )

        // properties will be correctly reordered above the methods
        ->addStmt($factory->property('someProperty')->makeProtected())
        ->addStmt($factory->property('anotherProperty')->makePrivate()->setDefault(array(1, 2, 3)))
    )

    ->getNode()
;

$stmts = array($node);
$prettyPrinter = new PrettyPrinter\Standard();
echo $prettyPrinter->prettyPrintFile($stmts);
// test
class instrumentation_var extends NodeVisitorAbstract{
    function __construct($type,$node) {
        $target = $node;
        $target_type = $type;
        //print "var instrumentor created";

    }
    public function leaveNode(Node $node) {
        if ($node instanceof Node\Stmt\Return_ && $node->expr !== null) {
            $var = new Node\Expr\Variable('tmp');
            $random_node = get_random_node();
            return [
                new Node\Stmt\Expression(new Node\Expr\Assign($var, $random_node)),
                new Node\Stmt\Return_($var),
            ];
        }
        global $factory;
        if ($node instanceof Node\Expr\Variable && $node == $this->target) {
            $random_node = get_random_node();
            $func_node = $factory->funcCall($random_node->Name, [$node->variable]);
            $var_node = $factory->var($node->variable);
            return [
                new Node\Stmt\Expression(new Node\Expr\Assign($var_node, $func_node)),
            ];
        }

    }
}

?>