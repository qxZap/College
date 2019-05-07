<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Document</title>
</head>
<body>
<h3>Cod secret: 1 2 3</h3>
<form action="lab6.php" method="post">
    <button type="submit" name="one">1</button>
    <button type="submit" name="two">2</button>
    <button type="submit" name="three">3</button>
    <input id="submit" style="display:none" type="submit">
</form>
<script>


</script>
<?php
error_reporting(0);
ini_set('display_errors', 0);
$secretCode="123";

if($_COOKIE["code"]==""){
    if(isset($_POST['one'])){
        setcookie("code", $_COOKIE["code"]."1", time()+3600, "/","", 0);
    }else{
        echo "<p>wrong code</p>";
        setcookie( "code", "", time()- 60, "/","", 0);
    }
}

if($_COOKIE["code"]=="1"){
    if(isset($_POST['two'])){
        setcookie("code", $_COOKIE["code"]."2", time()+3600, "/","", 0);
    }else{
        echo "<p>wrong code</p>";
        setcookie( "code", "", time()- 60, "/","", 0);
    }
}
if( $_COOKIE["code"]=="12"){
    if(isset($_POST['three'])){
        echo "<p>Perfect !</p>";
        setcookie( "code", "", time()- 60, "/","", 0);
    }
    else{
        echo "<p>wrong code</p>";
        setcookie( "code", "", time()- 60, "/","", 0);
    }   

}

?>


</body>
</html>