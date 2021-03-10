<?php
/**
 * Implementation of a call graph generation strategy wich outputs the calls as
 * plain text.
 *
 * PHP version 5
 *
 * This file is part of phpCallGraph.
 *
 * PHPCallGraph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * PHPCallGraph is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @package    PHPCallGraph
 * @author     Falko usere <fakko at users dot sourceforge dot net>
 * @copyright  2007-2009 Falko usere
 * @license    http://www.gnu.org/licenses/gpl.txt GNU General Public License
 */

require_once 'CallgraphDriver.php';

/**
 * Implementation of a call graph generation strategy wich outputs the calls as
 * plain text.
 */

class TextDriver implements CallgraphDriver {

    /**
     * @var string
     */
    protected $text = '';

    /**
     * @var boolean
     */
    protected $verbose;

    /**
     * @param boolean $verbose
     * @return CallgraphDriver
     */
    public function __construct($verbose = false) {
        $this->verbose = $verbose;
    }

    /**
     * @param integer $line
     * @param string $file
     * @param string $name
     * @return void
     */
    public function startFunction($line, $file, $name, $memberCode) {
        $this->text.= $name;
        if ($this->verbose) {
            $this->text.= " defined in $file on line $line";
        }
        $this->text.= "\n";
    }

    /**
     * @param integer $line
     * @param string $file
     * @param string $name
     * @return void
     */
    public function addCall($line, $file, $name) {
        $this->text.= "\t$name";
        if ($this->verbose) {
            $this->text.= " -- called on line $line and defined in $file";
        }
        $this->text.= "\n";
    }

    /**
     * @return void
     */
    public function endFunction() {
    }

    /**
     * @return string
     */
    public function __toString() {
        return $this->text;
    }

    /**
     * @return void
     */
    public function reset() {
        $this->text = '';
    }

}
