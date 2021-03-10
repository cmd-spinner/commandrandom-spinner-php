# **Supplementary Materials**

## 1. Usability of Trusted Command Specifications Generator

We provide an automated tool that generates the trusted command specification and details about this process.

The below example is one of the inputs we used during the experiment.

```javascript
{
  "shell"  : ["passthru", "system", "popen", "shell_exec", "exec", "proc_open"],
  "db"     : ["mysqli::multi_query", "mysqli::prepare", "mysqli::real_query", "mysqli::send_query", "mysql_query"],
  "xml"    : ["simplexml_load_file"],
  "config" : ["ini_get"],
  "env"    : ["getenv"],
  "folder" : ""
}
```

- More details with inputs for all target applications can be found on [**`Trusted Specifications Sub Page`**](./Trusted%20Specifications/)

## 2. Static/Dynamic Randomization

We tried brute-force attacks on static/dynamic randomization approaches. Using the dynamic approach (1-to-1 mapping) for three characters-long commands requires 70,191 more attempts to succeed the attack than the static approach. In general, we observe that the dynamic randomization approach makes the search space for the attacker twice larger than the static randomization approach.

| Scheme           | Static randomization | Dynamic randomization |
| ---------------- | -------------------: | --------------------: |
| 1 to 1 (x ↦ y)   |                71.1K |                141.3K |
| 1 to 2 (x ↦ xy)  |                 9.8M |                 19.7M |
| 1 to 3 (x ↦ xyz) |             1,389T\* |              2,779T\* |

> \*: Estimated value (unit: trillion): We decided to use the estimation because the experiment did not finish within 10 hours. We observe this result follows the distribution (i.e., static randomization approach follows the uniform distribution and dynamic randomization approach follows the geometric distribution). According to this observation, we put the expected value though the statistical method.

- Details can be found on [**`Brute Force Attack Sub Page`**](./BruteForceAttack/).

## 3. Evaluation

We provide additional information about our evaluation including the **evaluated programs' versions** and **the process of evaluation for bidirectional analysis's accuracy**.

- Details can be found on [**`Evaluation Sub Page`**](./Evaluation).

## 4. Database

Databases are typically _untrusted_, while they can be trusted if there is _no data flow from untrusted sources to the database_. Our tool can make such decisions.
We provide the explanation with examples related to second-order SQL injections in this section.

- Details can be found on [**`Database Sub Page`**](./Is%20Database%20Trusted%20Untrusted).
