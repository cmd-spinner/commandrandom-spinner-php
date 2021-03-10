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

class BadSqlTainter implements AfterExpressionAnalysisInterface
{
    /**
     * Called after an expression has been checked
     *
     * @param  PhpParser\Node\Expr  $expr
     * @param  Context              $context
     * @param  string[]             $suppressed_issues
     * @param  FileManipulation[]   $file_replacements
     *
     * @return void
     */
    public static function afterExpressionAnalysis($expr,$context,$statements_source,$codebase,array &$file_replacements = []) {
        if ($expr instanceof PhpParser\Node\Scalar\String_) {
            $expr_type = $statements_source->getNodeTypeProvider()->getType($expr);
            var_dump($expr->value);
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