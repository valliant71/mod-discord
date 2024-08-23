# Mount at Twenty

## Description

This module automatically grants players a race-specific mount and the Apprentice Riding skill when they reach a specified level (default is 20) in World of Warcraft.

## Features

-   Automatically teaches Apprentice Riding skill at the specified level
-   Sends a race-specific mount to the player via in-game mail
-   Configurable through the `mount-at-twenty.conf` file and database entries
-   Customizable mail subject and text

## Installation

1. Place the module in the `modules` directory of your AzerothCore source.
2. Import the `mount_at_twenty_world.sql` file into your **world database**.
3. Re-run cmake and launch a clean build of AzerothCore.

## Configuration

### my_custom.conf

The module can be configured in the `mount-at-twenty.conf` file:

```ini
MountAtTwenty.Enable = 1
```

### Database Configuration

You can customize the mounts for each race by modifying the `mount_at_twenty_races` table in the world database. You can also adjust the module settings in the `module_config` table.

## Database Tables

The following tables are added to the **world database**:

-   `mount_at_twenty_races`: Defines the mount item and spell for each race.
-   `module_config`: Contains module-specific configuration options.

## Credits

-   Module developed by Lodef
-   AzerothCore: [repository](https://github.com/azerothcore/azerothcore-wotlk)

## License

This module is released under the [GNU AGPL license](https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3).
