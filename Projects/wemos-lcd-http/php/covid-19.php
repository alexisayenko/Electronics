<?php
include_once('simple_html_dom.php');

// COVID-19
$html = file_get_html('https://www.worldometers.info/coronavirus/');

if (empty($html))
    echo "- no data -";
else
    echo $html->find('.maincounter-number')[0]->children[0]->innertext;
?>