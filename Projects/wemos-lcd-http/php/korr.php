<?php
# include_once('simple_html_dom.php');
echo "Последние новости длинный текст.";
$newsoutput = new SimpleXMLElement('http://k.img.com.ua/rss/ru/all_news2.0.xml', true);
echo $newsoutput; #$html->find('item')[0]->children[0]->innertext;

?>