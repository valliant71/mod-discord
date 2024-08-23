#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "SpellMgr.h"
#include "MailSender.h"
#include "Item.h"
#include "ObjectMgr.h"
#include "DatabaseEnv.h"

class MountAtTwenty : public PlayerScript
{
public:
    MountAtTwenty() : PlayerScript("MountAtTwenty") { }

    void OnLevelChanged(Player* player, uint8 oldLevel) override
    {
        // Check if the module is enabled
        if (!sConfigMgr->GetOption<bool>("MountAtTwenty.Enable", true))
            return;

        // Get the level at which to grant the mount
        uint32 mountLevel = sConfigMgr->GetOption<uint32>("MountAtTwenty.Level", 20);

        // Check if the player has reached the required level
        if (oldLevel < mountLevel && player->getLevel() >= mountLevel)
        {
            // Teach riding skill
            player->learnSpell(33388); // Apprentice Riding

            // Send race-specific mount
            SendRaceSpecificMount(player);
        }
    }

private:
    void SendRaceSpecificMount(Player* player)
    {
        uint32 mountEntry = 0;
        uint32 mountSpellId = 0;

        // Query the database for the race-specific mount
        QueryResult result = WorldDatabase.Query("SELECT mount_entry, mount_spell_id FROM mount_at_twenty_races WHERE race = {}", player->getRace());
        if (result)
        {
            Field* fields = result->Fetch();
            mountEntry = fields[0].Get<uint32>();
            mountSpellId = fields[1].Get<uint32>();
        }
        else
        {
            // Fallback to default mount if not found in the database
            mountEntry = GetDefaultMountEntry(player->getRace());
            mountSpellId = GetDefaultMountSpellId(player->getRace());
        }

        if (!mountEntry || !mountSpellId)
            return;

        // Create the mount item
        Item* mountItem = Item::CreateItem(mountEntry, 1, player);
        if (!mountItem)
            return;

        // Teach the mount spell
        player->learnSpell(mountSpellId);

        // Send the mount via mail
        std::string mailSubject = sConfigMgr->GetOption<std::string>("MountAtTwenty.MailSubject", "Your New Mount");
        std::string mailText = sConfigMgr->GetOption<std::string>("MountAtTwenty.MailText", "Congratulations on reaching level {level}! Here's your very own mount.");
        
        // Replace {level} in the mail text with the actual level
        size_t pos = mailText.find("{level}");
        if (pos != std::string::npos)
            mailText.replace(pos, 7, std::to_string(player->getLevel()));

        MailSender sender(MAIL_NORMAL, player->GetGUID().GetCounter(), MAIL_STATIONERY_GM);
        MailDraft draft(mailSubject, mailText);
        draft.AddItem(mountItem);
        draft.SendMailTo(MailReceiver(player), sender);
    }

    uint32 GetDefaultMountEntry(uint8 race)
    {
        switch (race)
        {
            case RACE_HUMAN:        return 6648;  // Brown Horse
            case RACE_ORC:          return 6653;  // Dire Wolf
            case RACE_DWARF:        return 6777;  // Gray Ram
            case RACE_NIGHTELF:     return 8394;  // Striped Nightsaber
            case RACE_UNDEAD_PLAYER:return 17462; // Red Skeletal Horse
            case RACE_TAUREN:       return 18989; // Brown Kodo
            case RACE_GNOME:        return 10969; // Blue Mechanostrider
            case RACE_TROLL:        return 10796; // Emerald Raptor
            case RACE_BLOODELF:     return 34795; // Red Hawkstrider
            case RACE_DRAENEI:      return 35713; // Brown Elekk
            default:                return 0;
        }
    }

    uint32 GetDefaultMountSpellId(uint8 race)
    {
        switch (race)
        {
            case RACE_HUMAN:        return 6648;  // Brown Horse
            case RACE_ORC:          return 6653;  // Dire Wolf
            case RACE_DWARF:        return 6777;  // Gray Ram
            case RACE_NIGHTELF:     return 8394;  // Striped Nightsaber
            case RACE_UNDEAD_PLAYER:return 17462; // Red Skeletal Horse
            case RACE_TAUREN:       return 18989; // Brown Kodo
            case RACE_GNOME:        return 10969; // Blue Mechanostrider
            case RACE_TROLL:        return 10796; // Emerald Raptor
            case RACE_BLOODELF:     return 34795; // Red Hawkstrider
            case RACE_DRAENEI:      return 35713; // Brown Elekk
            default:                return 0;
        }
    }
};

void AddMyPlayerScripts()
{
    new MountAtTwenty();
}
