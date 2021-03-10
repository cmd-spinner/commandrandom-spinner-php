<?php
namespace Psalm\Issue;

use Psalm\Internal\Analyzer\TaintNodeData;
use Psalm\CodeLocation;

class TaintedInput extends CodeIssue
{
    const ERROR_LEVEL = -2;
    const SHORTCODE = 205;

    /**
     * @var string
     * @readonly
     */
    public $journey_text;

    /**
     * @var list<array{location: ?CodeLocation, label: string, entry_path_type: string}>
     * @readonly
     */
    public $journey = [];

    /**
     * @param list<array{location: ?CodeLocation, label: string, entry_path_type: string}> $journey
     */
    public function __construct(
        string $message,
        CodeLocation $code_location,
        array $journey,
        string $journey_text
    ) {
        parent::__construct($message, $code_location);

        $this->journey = $journey;
        $this->journey_text = $journey_text;
    }

    /**
     * @return list<TaintNodeData|array{label: string, entry_path_type: string}>
     */
    public function getTaintTrace(): array
    {
        $nodes = [];

        foreach ($this->journey as ['location' => $location, 'label' => $label, 'entry_path_type' => $path_type]) {
            if ($location) {
                $nodes[] = self::nodeToTaintNodeData($location, $label, $path_type);
            } else {
                $nodes[] = ['label' => $label, 'entry_path_type' => $path_type];
            }
        }

        return $nodes;
    }

    private static function nodeToTaintNodeData(
        CodeLocation $location,
        string $label,
        string $entry_path_type
    ) : TaintNodeData {
        $selection_bounds = $location->getSelectionBounds();
        $snippet_bounds = $location->getSnippetBounds();

        return new TaintNodeData(
            $label,
            $entry_path_type,
            null,
            $location->getLineNumber(),
            $location->getEndLineNumber(),
            $location->file_name,
            $location->file_path,
            $location->getSnippet(),
            $location->getSelectedText(),
            $selection_bounds[0],
            $selection_bounds[1],
            $snippet_bounds[0],
            $snippet_bounds[1],
            $location->getColumn(),
            $location->getEndColumn()
        );
    }

    public function getJourneyMessage() : string
    {
        return $this->message . ' in path: ' . $this->journey_text;
    }
}
