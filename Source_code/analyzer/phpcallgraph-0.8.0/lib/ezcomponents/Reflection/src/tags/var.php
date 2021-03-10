<?php
/**
 * File containing the ezcReflectionDocTagVar class.
 *
 * @package Reflection
 * @version //autogen//
 * @copyright Copyright (C) 2005-2008 eZ systems as. All rights reserved.
 * @license http://ez.no/licenses/new_bsd New BSD License
 */

/**
 * Represents a var doc tag in the php source code comment.
 *
 * @package Reflection
 * @version //autogen//
 * @author Stefan Marr <mail@stefan-marr.de>
 */
class ezcReflectionDocTagVar extends ezcReflectionDocTag {

    /**
    * @param string[] $line Array of words
    */
    public function __construct($line) {
        $this->tagName = $line[0];

        if (isset($line[1])) {
            $this->params[0] = ezcReflectionTypeMapper::getInstance()->getType($line[1]);
        }
        if (isset($line[2])) {
            $this->desc = $line[2];
        }
        if (isset($line[3])) {
            $this->desc .= ' '.$line[3];
        }
    }

    /**
    * @return string
    */
    public function getType() {
        return $this->params[0];
    }
}
?>
