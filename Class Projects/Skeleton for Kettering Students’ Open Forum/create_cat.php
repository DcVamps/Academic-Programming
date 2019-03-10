<?php
//create_cat.php
include 'connect2.php';
include 'header.php';

if($_SERVER['REQUEST_METHOD'] != 'POST')
{
    echo '<h2>Create a New Category</h2>';
    //the form hasn't been posted yet, display it
    echo '<form method="post" action="">
        Category name: <br /><input type="text" name="cat_name" /><br />
        Category description: <br /><textarea name="cat_description" /></textarea><br />
        <input type="submit" value="Add category" />
     </form>';
}
else
{
    //the form has been posted, so save it
    $sql = "INSERT INTO
                cs341.categories(cat_name,
                            cat_description)
            VALUES('" . mysql_real_escape_string($_POST['cat_name']) . "',
             " . mysql_real_escape_string($_POST['cat_description']) . ")";
    $result = mysql_query($sql);
    if(!$result)
    {
        //something went wrong, display the error
        echo 'Error' . mysql_error();
    }
    else
    {
        echo 'New category successfully added.';
    }
}

include 'footer.php';
?>
