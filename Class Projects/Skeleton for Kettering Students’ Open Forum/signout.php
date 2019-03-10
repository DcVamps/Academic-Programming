<?php include 'header.php'; ?>
<?php
// remove all session variables
session_unset();

// destroy the session
session_destroy();
header('Location: mockup.php');
?>

<?php include 'footer.php'; ?>
