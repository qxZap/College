<?php 
    include_once './users.model.php';
    
    insertUser($_POST['username'], $_POST['pass']);
    header('Location: /lab6/lab6.php');

?>