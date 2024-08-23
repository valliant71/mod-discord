#include "ScriptMgr.h"
#include "Config.h"
#include "Chat.h"
#include "Channel.h"
#include "ChannelMgr.h"
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <vector>
#include <sstream>
#include <algorithm>

class DiscordLink : public WorldScript
{
public:
    DiscordLink() : WorldScript("DiscordLink") 
    {
        LoadChannelList();
    }

    void OnAfterConfigLoad(bool reload) override
    {
        if (reload)
        {
            LoadChannelList();
        }
    }

    void OnStartup() override
    {
        if (sConfigMgr->GetOption<bool>("DiscordLink.Enable", false))
        {
            RegisterChannelHooks();
        }
    }

private:
    std::vector<std::string> monitoredChannels;

    void LoadChannelList()
    {
        monitoredChannels.clear();
        std::string channelList = sConfigMgr->GetOption<std::string>("DiscordLink.Channels", "World,General");
        std::istringstream iss(channelList);
        std::string channel;
        while (std::getline(iss, channel, ','))
        {
            monitoredChannels.push_back(channel);
        }
    }

    void RegisterChannelHooks()
    {
        ChannelMgr* mgr = ChannelMgr::forTeam(TEAM_ALLIANCE); // Channels are faction-independent, so we can use either
        if (mgr)
        {
            for (const auto& channelName : monitoredChannels)
            {
                if (Channel* channel = mgr->GetJoinChannel(channelName, 0))
                {
                    channel->SetHandler(this);
                }
            }
        }
    }

    void OnChat(Channel* channel, Player* player, std::string& msg) override
    {
        if (std::find(monitoredChannels.begin(), monitoredChannels.end(), channel->GetName()) != monitoredChannels.end())
        {
            SendToDiscord(player->GetName(), channel->GetName(), msg);
        }
    }

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
    {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    void SendToDiscord(const std::string& playerName, const std::string& channelName, const std::string& message)
    {
        CURL* curl;
        CURLcode res;
        std::string readBuffer;

        curl = curl_easy_init();
        if (curl)
        {
            std::string webhookUrl = sConfigMgr->GetOption<std::string>("DiscordLink.WebhookURL", "");
            if (webhookUrl.empty())
            {
                LOG_ERROR("module", "DiscordLink: WebhookURL is not set in configuration.");
                return;
            }

            Json::Value payload;
            payload["content"] = "[" + channelName + "] " + playerName + ": " + message;

            std::string payloadStr = Json::FastWriter().write(payload);

            curl_easy_setopt(curl, CURLOPT_URL, webhookUrl.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payloadStr.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            struct curl_slist* headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
            {
                LOG_ERROR("module", "DiscordLink: Failed to send message to Discord: {}", curl_easy_strerror(res));
            }

            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }
    }
};

// Add all scripts in one
void AddDiscordLinkScripts()
{
    new DiscordLink();
}