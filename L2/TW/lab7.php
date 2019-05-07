	
<?php 
  $CONFIG = [
    'servername' => "localhost",
    'username' => "cliwapp",
    'password' => 'elburaga',
    'db' => 'cliwapp'
  ];
 
  $conn = new mysqli($CONFIG["servername"], $CONFIG["username"], $CONFIG["password"], $CONFIG["db"]);
 
  if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
  } else { 
    echo "Successfully connected to DB";
  }
?> 

<?php 
 $username = "Bob";
 $accesstoken = uniqid();

 $insertStatement = $conn -> prepare("INSERT INTO activation VALUES(?, ?)");
 $insertStatement -> bind_param("ss", $username, $accesstoken);

 $insertStatement -> execute();
 $insertStatement -> close();
?> 


<?php 
  $getUsersStatement = $conn -> prepare("SELECT * FROM users ");
  $getUsersStatement -> execute();
 
  $users = $getUsersStatement -> get_result();
 
  foreach($users as $row) {
      echo $row['username'];
      echo '<br/>';
  }
?>  