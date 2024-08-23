#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "SpellMgr.h"
#include "Mail.h"
#include "Item.h"
#include "ObjectMgr.h"
#include "DatabaseEnv.h"

class MountAtTwenty : public PlayerScript
{
public:
    MountAtTwenty() : PlayerScript("MountAtTwenty") { }

    void OnLogin(Player* player) override
    {
        // Check if the player's level is at or above the mount level
        if (player->getLevel() >= GetMountLevel())
        {
            // Check if the player already has riding skill
            if (!player->HasSpell(33388))
            {
                GrantMountAndSkill(player);
            }
        }
    }

    void OnLevelChanged(Player* player, uint8 oldLevel) override
    {
        // Check if the module is enabled
        if (!sConfigMgr->GetOption<bool>("MountAtTwenty.Enable", true))
            return;

        // Get the level at which to grant the mount
        uint32 mountLevel = GetMountLevel();

        // Check if the player has reached the required level
        if (oldLevel < mountLevel && player->getLevel() >= mountLevel)
        {
            GrantMountAndSkill(player);
        }
    }

private:
    void GrantMountAndSkill(Player* player)
    {
        // Teach riding skill
        player->learnSpell(33388); // Apprentice Riding

        // Send race-specific mount
        SendRaceSpecificMount(player);
    }

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

        MailDraft draft(mailSubject, mailText);
        draft.AddItem(mountItem);
        draft.SendMailTo(MailReceiver(player), MailSender(MAIL_NORMAL, 0, MAIL_STATIONERY_GM));
    }

    uint32 GetDefaultMountEntry(uint8 race)
    {
        switch (race)
        {
            case RACE_HUMAN:        return 2414;  // Black Stallion Bridle
            case RACE_ORC:          return 5665;  // Horn of the Dire Wolf
            case RACE_DWARF:        return 5864;  // Gray Ram
            case RACE_NIGHTELF:     return 8629;  // Reins of the Striped Nightsaber
            case RACE_UNDEAD_PLAYER:return 13331; // Red Skeletal Horse
            case RACE_TAUREN:       return 15277; // Gray Kodo
            case RACE_GNOME:        return 8563;  // Icy Blue Mechanostrider Mod A
            case RACE_TROLL:        return 8588;  // Whistle of the Emerald Raptor
            case RACE_BLOODELF:     return 29221; // Swift Green Hawkstrider
            case RACE_DRAENEI:      return 28481; // Brown Elekk
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

    uint32 GetMountLevel()
    {
        return sConfigMgr->GetOption<uint32>("MountAtTwenty.Level", 20);
    }
};

void AddMountAtTwentyScripts()
{
    new MountAtTwenty();
}