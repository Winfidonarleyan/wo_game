DROP TABLE IF EXISTS `spell_damage`;
CREATE TABLE `spell_damage` (
  `spellId` int(11) unsigned NOT NULL,
  `percentage` float NOT NULL DEFAULT 100,
  PRIMARY KEY (`spellId`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

INSERT INTO `spell_damage` VALUES ('41455', '1');
INSERT INTO `spell_damage` VALUES ('69399', '1000');
INSERT INTO `spell_damage` VALUES ('69400', '1000');
INSERT INTO `spell_damage` VALUES ('69401', '1000');
INSERT INTO `spell_damage` VALUES ('69402', '1000');
INSERT INTO `spell_damage` VALUES ('70172', '1000');
INSERT INTO `spell_damage` VALUES ('70173', '1000');
INSERT INTO `spell_damage` VALUES ('70174', '1000');
INSERT INTO `spell_damage` VALUES ('70175', '1000');
INSERT INTO `spell_damage` VALUES ('70542', '1000');
INSERT INTO `spell_damage` VALUES ('72292', '1');
