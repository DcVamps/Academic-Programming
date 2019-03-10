<?php
   include('connect2.php');
   $user_check = $_SESSION['signed_in'];
   $ses_sql = mysqli_query($db,"select username from admin where username = '$user_check' ");

   $row = mysqli_fetch_array($ses_sql,MYSQLI_ASSOC);

   $login_session = $row['username'];

   if(!isset($_SESSION['login_user'])){
      header("location:login.php");
   }
?>
