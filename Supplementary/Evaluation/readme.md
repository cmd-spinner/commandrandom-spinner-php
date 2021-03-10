# Evaluation

## Performance and correctness of recent versions

We applied Spinner to WordPress (v5.6.2; released on Feb 2021) and Leptonica (v1.8; the latest). The resulting protected programs are correct where we observe a similar average overhead of 4.31% (+0.01% from the versions in the paper).

| Testbeds  | Version used in the paper |     Latest version      |
| :-------- | :-----------------------: | :---------------------: |
| WordPress |           4.33%           |          4.41%          |
|           | (released in `12/18/19`)  | (released in `2/22/21`) |
| Leptonica |           4.25%           |          4.21%          |
|           |  (released in `6/11/17`)  | (released in `7/28/20`) |

All programs we used in the paper have version information, as shown below.

| ID  |  Version |     | ID  |    Version |     | ID  |      Version |
| :-- | -------: | --- | :-- | ---------: | --- | :-- | -----------: |
| s1  |    5.3.2 |     | s15 |      0.1.4 |     | s29 |         15.2 |
| s2  | 20161228 |     | s16 |      3.5.0 |     | s30 |        4.1.7 |
| s3  |      2.3 |     | s17 |      1.0.1 |     | s31 |       3.0.12 |
| s4  |     0.15 |     | s18 |      1.1.2 |     | s32 |          0.7 |
| s5  |      1.7 |     | s19 |      1.2.0 |     | s33 |        5.1.8 |
| s6  |    3.0.9 |     | s20 |      0.1.0 |     | s34 |        5.1.3 |
| s7  |   1.4.35 |     | s21 |      6.0.0 |     | s35 |        2.0.4 |
| s8  |   1.74.4 |     | s22 |      0.7.0 |     | s36 |        6.2.3 |
| s9  |    2.7.6 |     | s23 | 0.2.4-beta |     | s37 |        3.0.2 |
| s10 |    3.6.5 |     | s24 |      3.6.1 |     | s38 |       3.36.0 |
| s11 |     0.10 |     | s25 |      0.3.6 |     | s39 |       3.22.1 |
| s12 |   0.4.18 |     | s26 |     0.4.13 |     | s40 |        0.7.2 |
| s13 |    1.1.0 |     | s27 |      1.0.2 |     | s41 |        6.3.0 |
| s14 |    2.6.0 |     | s28 |      5.2.2 |     | s42 | 1.0.0-pre.45 |

## Analysis Accuracy

Since obtaining the ground-truth is challenging, we try the following to evaluate the bidirectional analysis’s accuracy. We manually verified that all the results from our analysis are true-positives. We also run other static/dynamic taint-analysis tools(i.e., `taintless`, `psalm` and `PECL taint`) and compare the output dependencies with the dependencies output by our tool. We observe that our tool covers majority of the dependencies chains that are output by the other tools. For dependencies not covered by our tool, we manually check them and find that they are false-positives (hence we are not missing anything covered by other tools).

> - We update the AST parser of `taintless` to the new version and add extra rules to help it handle WordPress's callback function hook.
> - We add additional plugins to `psalm` to enhance its ability on tracing data flow on object inheritance.
> - We add addtional WordPress specific sinks to `PECL taint` to enhance its ability on WordPress.

### Procedure of the Evaluation

We do our evaluation as follows.

1. Run the bi-directional analysis and manually verify the dependency chains identified by the analysis.

   a) Manually check the propagation rules applied by the bi-directional analysis (both forward and backward analyses).

   b) Verify that all the dependencies identified by the bi-directional analysis are **true-positives**.

2. Run other static/dynamic analysis tools to get dependency chains (Note that static/dynamic analysis tools suffer from over and under-approximations)

   a) If other tools find _more_ dependencies, then they might be potential _false-negatives_ of the bi-directional analysis.

   - We manually verify them all, and the result shows that they are all false-positives, meaning that we did not find false-negatives from the bi-directional analysis.

   b) If other tools find _lesser_ dependencies, then they might be potential _false-positives_ of the bi-directional analysis.

   - We manually verify them all, and the result shows that they are all false-negatives, meaning that we did not find false-positives from the bi-directional analysis.

### Procedure and Method for Manual Analysis

- Our manual analysis leverages existing static/dynamic analysis techniques. While they are inaccurate, we only apply them for a single dependency chain and reason about the result.
  - Since we only reason a single dependency at a time, the task was manageable even though it is a time-consuming task.
- We conduct inter-procedural manual analysis, meaning that we follow through the callee functions' arguments if values propagate through the functions.
- The analysis finishes when the data reaches a trusted/untrusted source.
- In addition to the static/dynamic taint analysis techniques, we manually run the programs and observe how the concrete values are propagated by changing inputs and checking output differences
  - Note that if an output value is changed from the above testing due to the input change, there is a dependency.

### The number of detected dependencies

| Testbeds                  | Bi-directional analysis | Taintless | Psalm | PECL taint |
| :------------------------ | ----------------------: | --------: | ----: | ---------: |
| WordPress\*               |                     462 |       413 |   426 |        537 |
| Activity Monitor\*        |                      27 |        16 |    17 |         27 |
| Avideo Encoder\*          |                      61 |        66 |    61 |         61 |
| Pepperminty WiKi          |                       2 |         2 |     2 |          2 |
| PHPSHE\*                  |                     270 |       301 |   266 |        223 |
| Pie Register\*            |                      73 |        79 |    77 |         73 |
| Contact-Form-7            |                       5 |         5 |     5 |          5 |
| Yoast SEO                 |                      27 |        27 |    27 |         27 |
| Akismet Spam Protection   |                      17 |        17 |    17 |         17 |
| Elementor Website Builder |                      23 |        23 |    23 |         23 |
| WordPress Importer        |                       2 |         2 |     2 |          2 |
| Symfony Console           |                      18 |        18 |    18 |         18 |
| Environment               |                       3 |         3 |     3 |          3 |
| Composer                  |                       8 |         8 |     8 |          8 |
| Swiftmailer               |                       1 |         1 |     1 |          1 |
| Version                   |                       1 |         1 |     1 |          1 |

\* Except for these 6 applications, there is no difference between the tools.

To make sure Spinner's bi-directional analysis does not miss anything, we compared the results with existing techniques (`Taintless`, `Psalm`, and `PECL taint`). We manually analyzed them and verified that all the results from bi-directional analysis are true-positives. Details are as follows.

#### WordPress

- Bi-directional vs. Taintless
  - Taintless has 49 false negatives.
    - 24 false negatives are caused as described in Figure 15.
    - 5 false negatives are caused by handling PHP dynamic function call (e.g., `call_user_func_array()`).
    - 20 false negatives are caused by handling WordPress `apply_filter` which invokes a function by the nickname registered by `add_filter`.
- Bi-directional vs. Psalm
  - Psalm has 24 false negatives as described in Figure 15.
  - Psalm is not accurate in handling object inheritance. It will miss the data dependencies from subclass methods to base class methods in 36 cases.
- Bi-directional vs. PECL taint
  - PECL taint has 35 false positives caused by handling WordPress `do_action` dynamic function hook.
  - PECL taint has 40 false positives caused by string array filtering operation.

#### Activity Monitor

- Bidirectional Analysis vs. Taintless
  - Taintless has 11 false negatives.
    - 3 false negatives are caused as shown in Figure 15.
    - 8 false negatives are caused by not supporting WordPress `apply_filter` which invoke a function registered by `add_filter` dynamically. The data flow will be broken when it goes into such APIs.
- Bidirectional Analysis vs. Psalm
  - Psalm has 14 false negative and 4 false positive cases.
    - 3 false negatives are caused as shown in Figure 15.
    - 8 false negatives are caused by `add_filter` and `apply_filter`.
    - 3 false negatives are caused by mishandling object inheritance. Variables defined in base class will not be recognized in subclass.
    - 4 false positive cases are caused by mishandling regex matching API `preg_match`.
- Bidirectional Analysis vs. PECL taint
  - Same result

#### Avideo-Encoder

- Bidirectional Analysis vs. Taintless
  - Taintless has 2 false negatives and 7 false positives.
    - 2 false negatives are caused by unsupported API `DateTime()` which should be considered as trusted.
    - 7 false positives are caused by mishandling regex API `preg_match`.
- Bidirectional Analysis vs. Psalm
  - Same result
- Bidirectional Analysis vs. PECL taint
  - Same result

#### PHPSHE

- Bidirectional Analysis vs. Taintless
  - Taintless has 16 false negatives and 47 false positives.
    - 16 false negatives are caused by parsing error on one PHP file. Internal bug on an old version of PHP-Parser.
    - 47 false positives are caused by history upgrading scripts.
- Bidirectional Analysis vs. Psalm
  - Psalm has 15 false negatives and 11 false positives
    - 3 false negatives are caused by `time()` API.
    - 12 false negatives are caused by class object inheritance.
    - 11 false positives are caused by SQL keywords in arguments used matching pattern of `preg_match` functions.
- Bi-directional vs. PECL taint
  - PECL taint has 47 false negatives because of PHP fatal error in executing database update script

#### Pie-register

- Bidirectional Analysis vs. Taintless
  - Taintless has 2 false negatives and 8 false positives.
    - 2 false negatives are caused by the case shown in Figure 15.
    - 8 false positives are caused by SQL keywords in the embedded HTML while they are not SQL statements.
- Bidirectional Analysis vs. Psalm
  - Psalm has 2 false negatives caused by the case shown in Figure 15.
- Bidirectional Analysis vs. PECL taint
  - Same result
