<?php
session_start();
?>
<html>
    <head>
        <title>Kettering Open Forum</title>
        <link rel = "stylesheet" type = "text/css" href = "style.css" media = "screen">
        <script src = "timeanddate.js"></script>
    </head>
    <body>
        <div id = "maincontent">
            <h1>Kettering University Open Forum</h1>
            <div id = "userInfoHeader">
                Welcome! &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                <?php
                    //include "signin.php";
                    if(isset($_SESSION['signed_in']) || $_SESSION['signed_in'] == true)
                    {
                        echo 'Hello &nbsp;', $_SESSION['user_name'], '&nbsp;<a href = "signout.php">Signout</a>';
                    }
                    else
                    {
                        include "signin.php";
                    }
                ?>
             <input id = "searchForum" type = "search" placeholder = "search"><br>
                <p>
                    <!--Add breadcrumb navigation here -->
                    <a href = "mockup.php">Home</a> &nbsp;&nbsp;
                    <a href = "create_cat.php">Create New Category</a>&nbsp;&nbsp;
                    <a href = "create_topic.php">Create New Topic</a>&nbsp;&nbsp;
                    <!--<a href = "#">Profile</a>-->
                </p>
            </div>
            <div id = "forumsection">
