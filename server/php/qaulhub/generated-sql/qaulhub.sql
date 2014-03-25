
# This is a fix for InnoDB in MySQL >= 4.1.x
# It "suspends judgement" for fkey relationships until are tables are set.
SET FOREIGN_KEY_CHECKS = 0;

-- ---------------------------------------------------------------------
-- msg
-- ---------------------------------------------------------------------

DROP TABLE IF EXISTS `msg`;

CREATE TABLE `msg`
(
    `id` INTEGER NOT NULL AUTO_INCREMENT,
    `type` INTEGER NOT NULL,
    `name` VARCHAR(255) NOT NULL,
    `msg` VARCHAR(255) NOT NULL,
    `ip` VARCHAR(255) NOT NULL,
    `time` VARCHAR(255) NOT NULL,
    `status` INTEGER NOT NULL,
    `twitterid` VARCHAR(255),
    `created_at` DATETIME,
    `updated_at` DATETIME,
    PRIMARY KEY (`id`),
    INDEX `twitterid` (`twitterid`)
) ENGINE=InnoDB;

-- ---------------------------------------------------------------------
-- file
-- ---------------------------------------------------------------------

DROP TABLE IF EXISTS `file`;

CREATE TABLE `file`
(
    `id` INTEGER NOT NULL AUTO_INCREMENT,
    `hash` VARCHAR(255) NOT NULL,
    `suffix` VARCHAR(255) NOT NULL,
    `description` VARCHAR(255) NOT NULL,
    `size` INTEGER NOT NULL,
    `time` VARCHAR(255) NOT NULL,
    `status` INTEGER NOT NULL,
    `created_at` DATETIME,
    `updated_at` DATETIME,
    PRIMARY KEY (`id`),
    INDEX `hash` (`hash`)
) ENGINE=InnoDB;

# This restores the fkey checks, after having unset them earlier
SET FOREIGN_KEY_CHECKS = 1;
