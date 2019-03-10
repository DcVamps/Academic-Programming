<?php
//create_cat.php
include 'connect2.php';


$sql = "SELECT *
        FROM
            cs341.categories";

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
        while($row = mysql_fetch_row($result))
        {
            echo '<h2> ′' . $row[1] . '′ </h2>';


        //do a query for the topics
        $sql2 = "SELECT *
                FROM
                    cs341.topics
                WHERE
                    topic_cat = '".$row[0]."'";

        $result2 = mysql_query($sql2);

        if(!$result2)
        {
            echo 'The topics could not be displayed, please try again later.';
        }
        else
        {
            if(mysql_num_rows($result2) == 0)
            {
                echo 'There are no topics in this category yet.';
            }
            else
            {
                //prepare the table
                echo '<table>
                <thead>
                    <td width = "25%">Topic</td>
                    <td width = "65%">Description</td>
                    <td width = "10%">Last Post</td>
                </thead>';

                while($row2 = mysql_fetch_row($result2))
                {
                    echo '<tr>';
                        echo '<td class="leftpart">';
                            echo '<h3><a href="topic.php?id=' . $row2[0] . '">' . $row2[1] . '</a><h3>';
                        echo '</td><td>' . $row2[5] . '</td>';
                        echo '<td class="rightpart">';
                            echo date('m-d-Y', strtotime($row2[2]));
                        echo '</td>';
                    echo '</tr>';
                }
                echo '<table>';
            }
        }
		mysql_free_result($result2);
    }
    }
}
?>
