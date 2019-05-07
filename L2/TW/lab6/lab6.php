<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>USERS!!!</title>
    <style>
     tr,td {
         border: 1px solid black;
     }
    </style>


</head>
<body>
<table>
    <thead>
        <tr>
            <th> Username </th>
            <th> Pass </th>
        </tr>
    </thead>
    <tbody>
        <?php 
            $fileContent = file_get_contents('./users.json');
            $users = json_decode($fileContent);

            foreach($users as $user) {
                echo '<tr>
                    <td>'. $user -> username .'</td>
                    <td>'. $user -> password .'</td>
                </tr>';
            }
        ?>
    </tbody>
</table>
<form action="./lab6.api.php" method="POST" class="register">
    <div>
        <label for="username">Username</label>
        <input id="username" placeholder="Username" type="text" name="username">
    </div>
    <div>
        <label for="pass">Password </label>
        <input id="pass" placeholder="Password" type="password" name="pass">
    </div>
    <button  name="submit" value="newUser" type="submit"> Create new user </button>
</form>


</body>
</html>