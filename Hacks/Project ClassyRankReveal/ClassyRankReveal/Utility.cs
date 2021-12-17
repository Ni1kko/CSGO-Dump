 
using System.Windows.Forms;

namespace GameInfoPlus
{ 
    public static class Utility
    {
        public static string[] RankArray = new string[] { "Unranked", "Silver I", "Silver II", "Silver III", "Silver IV", "Silver Elite", "Silver Elite Master",
        "Gold Nova I", "Gold Nova II", "Gold Nova III","Gold Nova Master",
        "Master Guardian I", "Master Guardian II", "Master Guardian Elite", "Distinguished Master Guardian", "Legendary Eagle", "Legendary Eagle master", "Supreme", "Global Elite"};
         
        public static void DisplayErrorMessage(string Message, string Caption)
        {
            MessageBox.Show(Message, Caption, MessageBoxButtons.OK, MessageBoxIcon.Error);
        } 

        public static string GetRankNameFromNumber(int RankID)
        {
            if (RankID >= 0 && RankID <= RankArray.Length)
            {
                return RankArray[RankID];
            }
           
            return "";
        }
    }
}
