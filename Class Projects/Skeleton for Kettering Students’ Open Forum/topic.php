<?php
//create_cat.php
include 'connect2.php';
include 'header.php';

//first select the category based on $_GET['cat_id']
$sql = "SELECT
            topic_id,
            topic_subject
        FROM
            cs341.topics
        WHERE
            topics.topic_id = " . mysql_real_escape_string($_GET['id']);

$result = mysql_query($sql);

if(!$result)
{
    echo 'The topic could not be displayed, please try again later.' . mysql_error();
}
else
{
    if(mysql_num_rows($result) == 0)
    {
        echo 'This topic does not exist.';
    }
    else
    {
        //display category data
        while($row = mysql_fetch_assoc($result))
        {
            echo '<h2> ' . $row['topic_subject'] . ' </h2>';
        }

        $sql = "SELECT
                    posts.post_topic,
                    posts.post_content,
                    posts.post_date,
                    posts.post_by,
                    users.user_id,
                    users.user_name
                FROM
                    cs341.posts
                LEFT JOIN
                    cs341.users
                ON
                    posts.post_by = users.user_id
                WHERE
                    posts.post_topic = " . mysql_real_escape_string($_GET['id']);

        $result = mysql_query($sql);

        if(!$result)
        {
            echo 'The posts could not be displayed, please try again later.';
        }
        else
        {
            if(mysql_num_rows($result) == 0)
            {
                echo 'There are no posts in this category yet.';
            }
            else
            {
                //prepare the table
                echo '<table>
                        <tr>
                        <th>By/Time</th>
                        <th>Post</th>
                      </tr>';

                while($row = mysql_fetch_assoc($result))
                {
                    echo '<tr>';
                        echo '<td>';
                            echo 'By ' . $row['user_name'] . ' on ' . $row['post_date'];
                        echo '</td><td>' . $row['post_content'] . '</td>';
                    echo '</tr>';
                }
                echo '</table>';
            }
        }
    }
}
?>

<?php include 'reply.php'; ?>
<?php include 'footer.php';
?>
