<?php

$domOBJ = new DOMDocument();
$domOBJ->load("http://k.img.com.ua/rss/ru/all_news2.0.xml");

$content = $domOBJ->getElementsByTagName("item");

$title = $content->item(0)->getElementsByTagName("title")->item(0)->nodeValue;

$title = transliterator_transliterate("Any-Latin; NFD; [:Nonspacing Mark:] Remove;", $title);

$title = str_replace("ʹ", "'", $title);

echo $title;

?>