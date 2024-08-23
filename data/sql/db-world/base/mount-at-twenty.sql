-- Create a table for race-specific mounts
CREATE TABLE IF NOT EXISTS `mount_at_twenty_races` (
  `race` TINYINT UNSIGNED NOT NULL COMMENT 'Race ID',
  `mount_entry` MEDIUMINT UNSIGNED NOT NULL COMMENT 'Mount item entry ID',
  `mount_spell_id` MEDIUMINT UNSIGNED NOT NULL COMMENT 'Mount spell ID',
  PRIMARY KEY (`race`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Insert default values for each race
INSERT INTO `mount_at_twenty_races` (`race`, `mount_entry`, `mount_spell_id`) VALUES
(1, 2414, 6648),   -- Human - Black Stallion Bridle
(2, 5665, 6653),   -- Orc - Horn of the Dire Wolf
(3, 5864, 6777),   -- Dwarf - Gray Ram
(4, 8629, 8394),   -- Night Elf - Reins of the Striped Nightsaber
(5, 13331, 17462), -- Undead - Red Skeletal Horse
(6, 15277, 18989), -- Tauren - Gray Kodo
(7, 8563, 10969),  -- Gnome - Icy Blue Mechanostrider Mod A
(8, 8588, 10796),  -- Troll - Whistle of the Emerald Raptor
(10, 29221, 34795),-- Blood Elf - Swift Green Hawkstrider
(11, 28481, 35713) -- Draenei - Brown Elekk
ON DUPLICATE KEY UPDATE
  `mount_entry` = VALUES(`mount_entry`),
  `mount_spell_id` = VALUES(`mount_spell_id`);

-- Add module configuration options
DELETE FROM `module_config` WHERE `module` = 'MountAtTwenty';
INSERT INTO `module_config` (`module`, `config`, `value`, `description`) VALUES
('MountAtTwenty', 'MountAtTwenty.Enable', '1', 'Enable the Mount at Twenty module'),
('MountAtTwenty', 'MountAtTwenty.Level', '20', 'Level at which to grant the mount and riding skill'),
('MountAtTwenty', 'MountAtTwenty.MailSubject', 'Your New Mount', 'Subject of the mail sent with the mount'),
('MountAtTwenty', 'MountAtTwenty.MailText', 'Congratulations on reaching level {level}! Here''s your very own mount.', 'Text of the mail sent with the mount');