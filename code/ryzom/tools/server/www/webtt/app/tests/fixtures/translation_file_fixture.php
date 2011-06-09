<?php
/* TranslationFile Fixture generated on: 2011-05-29 19:13:14 : 1306689194 */
class TranslationFileFixture extends CakeTestFixture {
	var $name = 'TranslationFile';

	var $fields = array(
		'id' => array('type' => 'integer', 'null' => false, 'default' => NULL, 'length' => 10, 'key' => 'primary'),
		'language_id' => array('type' => 'integer', 'null' => true, 'default' => NULL, 'length' => 10),
		'filename' => array('type' => 'string', 'null' => true, 'default' => NULL, 'collate' => 'utf8_general_ci', 'charset' => 'utf8'),
		'created' => array('type' => 'datetime', 'null' => true, 'default' => NULL),
		'modified' => array('type' => 'datetime', 'null' => true, 'default' => NULL),
		'indexes' => array('PRIMARY' => array('column' => 'id', 'unique' => 1)),
		'tableParameters' => array('charset' => 'utf8', 'collate' => 'utf8_general_ci', 'engine' => 'MyISAM')
	);

	var $records = array(
		array(
			'id' => 1,
			'language_id' => 1,
			'filename' => 'Lorem ipsum dolor sit amet',
			'created' => '2011-05-29 19:13:14',
			'modified' => '2011-05-29 19:13:14'
		),
	);
}
