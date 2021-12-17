<?php
// Include required library files.
require_once('../includes/config.php');
require_once('../autoload.php');


// Create PayPal object.
$PayPalConfig = array(
					  'Sandbox' => $sandbox,
					  'DeveloperAccountEmail' => $developer_account_email,
					  'ApplicationID' => $application_id,
					  'DeviceID' => $device_id,
					  'IPAddress' => $_SERVER['REMOTE_ADDR'],
					  'APIUsername' => $api_username,
					  'APIPassword' => $api_password,
					  'APISignature' => $api_signature,
					  'APISubject' => $api_subject
					);

$PayPal = new angelleye\PayPal\Adaptive($PayPalConfig);

// Pass data into class for processing with PayPal and load the response array into $PayPalResult
$PayKey = 'AP-8GH334440A259981L';
$PayPalResult = $PayPal->GetPaymentOptions($PayKey);

// Write the contents of the response array to the screen for demo purposes.
echo '<pre />';
print_r($PayPalResult);
?>