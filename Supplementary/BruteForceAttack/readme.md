# Brute-force Attack Result

This page shows the results of brute-force attacks on static randomization scheme and Spinner's dynamic randomization scheme (Relevant section in the paper: 9.3.3 Brute-force Attack Spinner).

## Randomization scheme

We tried brute-force attacks to break the randomization scheme of 3 characters long command (e.g., `env`).
We assume a strong attacker who already knows that the Spinner is using 1-to-2 mapping randomization scheme (e.g., e
↦ gc, n ↦ kv, v ↦ mS in the first row of below table).
The below table shows a few records of failed brute force attempts.

| Idx   | Brute force attack trial | Rand. cmd |
| ----- | ------------------------ | --------- |
| ...   | ...                      | ...       |
| 66190 | QMzkDG                   | gckvmS    |
| 66191 | PcSCNP                   | bojdZH    |
| 66192 | gmCXXe                   | OVhCZV    |
| 66193 | PLttlS                   | SqzXdb    |
| ...   | ...                      | ...       |

## \# of attempts to successfully launch the attack

The below table shows the number of failed attempts before the first correct guess, leading to a successful attack.
For instance, using the 1 to 1 mapping scheme, the static method prevents 71.1K attempts successfully. With the dynamic randomization scheme, the attack has to run 141.3K commands until the first successful guess.

| Scheme | Static randomization | Dynamic randomization |
| ------ | -------------------: | --------------------: |
| 1 to 1 |                71.1K |                141.3K |
| 1 to 2 |                 9.8M |                 19.7M |
| 1 to 3 |             1,389T\* |              2,779T\* |
| 1 to 4 |       195,438,503T\* |        390,877,006T\* |

\*: Estimated value (unit: trillion): We decided to use the estimation because the experiment did not finish within 10 hours. We observe this result follows the distribution (i.e., static randomization approach follows the uniform distribution and dynamic randomization approach follows the geometric distribution). According to this observation, we put the expected value through the statistical method.

For the case of 1 to 2 scheme, using dynamic approaches for this command requires 9,807,906,470 more attempts to succeed the attack than static randomization.
