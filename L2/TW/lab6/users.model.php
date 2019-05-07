<?php 
    function insertUser($username, $password) {
        $contents = file_get_contents('./users.json');
        $users = json_decode($contents);
        $newContent = ['username'=>$username,'password'=>$password];
        array_push($users,$newContent);
        file_put_contents('./users.json', json_encode($users));
    }
?>