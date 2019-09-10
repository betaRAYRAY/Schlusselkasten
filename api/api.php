<?php
if ($_GET['apikey'] ==  'SecretAPIKey') {
        $file = file("path/to/user.csv");
        $len =  count($file);
        echo $len . "\n";
        for ($i = 0; $i < $len; $i++) {
                echo explode(";", $file[$i] )[0];
                echo "\n";
        }
}
else {
        echo "Wrong API Key";
}

/*
USER.csv must be in this Format
Hex of UID in lowercase;name
Example:

12bb4f00;MaxMuster
bb420975;ErikaMuster
89e370ae;JaneDoe


*/
?>

