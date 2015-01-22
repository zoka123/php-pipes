<?php

const IN = 0;
const OUT = 1;

$descriptorspec = array(
    IN  => array("pipe", "r"),  // stdin is a pipe that the child will read from
    OUT => array("pipe", "w"),  // stdout is a pipe that the child will write to
    2   => array("pipe", "w"),
);

$process = proc_open('bc', $descriptorspec, $pipes);
if (is_resource($process)) {

    $inputFile = fopen("tasks.txt", "r");

    if ($inputFile) {
        while (($line = fgets($inputFile)) !== false) {

            echo $line;
            $userInput = rtrim(fgets(STDIN));

            // Write to child's IN pipe and get answer from child's OUT pipe
            fwrite($pipes[IN], $line);
            $answer = rtrim(fgets($pipes[OUT]));

            if (!is_numeric($answer)) {
                echo chr(27) . "[41m" . " NEISPRAVAN IZRAZ " . $answer . chr(27) . "[0m" . PHP_EOL;
            } else {
                if ($answer == $userInput) {
                    echo chr(27) . "[42m" . " ISPRAVNO " . chr(27) . "[0m" . PHP_EOL;
                } else {
                    echo chr(27) . "[41m" . " NEISPRAVNO točan odgovor je {$answer}" . chr(27) . "[0m" . PHP_EOL;
                }
            }
        }
    }

    fclose($pipes[IN]);
    fclose($pipes[OUT]);
    $return_value = proc_close($process);

    // It is important that you close any pipes before calling
    // proc_close in order to avoid a deadlock
    echo "command returned $return_value\n";


}

