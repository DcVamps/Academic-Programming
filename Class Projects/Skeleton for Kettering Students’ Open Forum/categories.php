<?php
//create_cat.php
include 'connect2.php';
include 'header.php';

//first select the category based on $_GET['cat_id']
$sql = "SELECT
            cat_id,
            cat_name,
            cat_description
        FROM
            cs341.categories
        WHERE
            cat_id = " . mysql_real_escape_string($_GET['id']);

$result = mysql_query($sql);

if(!$result)
{
    echo 'The category could not be displayed, please try again later.' . mysql_error();
}
else
{
    if(mysql_num_rows($result) == 0)
    {
        echo 'This category does not exist.';
    }
    else
    {
        //display category data
        while($row = mysql_fetch_assoc($result))
        {
            echo '<h2> ' . $row['cat_name'] . ' </h2>';
        }

        //do a query for the topics
        $sql = "SELECT
                    topic_id,
                    topic_subject,
                    topic_date,
                    topic_cat,
                    topic_desc
                FROM
                    cs341.topics
                WHERE
                    topic_cat = " . mysql_real_escape_string($_GET['id']);

        $result = mysql_query($sql);

        if(!$result)
        {
            echo 'The topics could not be displayed, please try again later.';
        }
        else
        {
            if(mysql_num_rows($result) == 0)
            {
                echo 'There are no topics in this category yet.';
            }
            else
            {
                //prepare the table
                echo '<table>
                <thead>
                    <td>Topic</td>
                    <td>Description</td>
                    <td>Last Post</td>
                </thead>';

                while($row = mysql_fetch_assoc($result))
                {
                    echo '<tr>';
                        echo '<td class="leftpart">';
                            echo '<h3><a href="topic.php?id=' . $row['topic_id'] . '">' . $row['topic_subject'] . '</a><h3>';
                        echo '</td><td>' . $row['topic_desc'] . '</td>';
                        echo '<td class="rightpart">';
                            echo date('d-m-Y', strtotime($row['topic_date']));
                        echo '</td>';
                    echo '</tr>';
                }
            }
        }
    }
}

include 'footer.php';
?>
