using System; 
using System.ComponentModel; 
using System.Threading; 
using System.Windows.Forms;
using GameInfoPlus.Classes;

namespace GameInfoPlus
{
    public partial class MainForm : Form
    {

        public void InitDataGrid()
        {
            dataGridView1.ColumnCount = 5;
             
            dataGridView1.Columns[0].Name = "Team";
            dataGridView1.Columns[1].Name = "Name";
            dataGridView1.Columns[2].Name = "Flashed"; 
            dataGridView1.Columns[3].Name = "Rank";
            dataGridView1.Columns[4].Name = "Wins";

            
        }
        public MainForm()
        {
            InitializeComponent();
            InitDataGrid();

            new Thread(() =>
            {
                Thread.CurrentThread.IsBackground = false;
                Thread.CurrentThread.Priority = ThreadPriority.Highest;
                while (true)
                {
                    Thread.Sleep(1000);
                    if (!autoUpdate) continue;
                    updateDataGrid(); 
                }
            }).Start();
        }

       private bool autoUpdate = false;

       private void updateDataGrid()
        {
            dataGridView1.Invoke(new Action(() => { dataGridView1.Rows.Clear(); }));

            int TotalCTs = 0;
            int TotalTs = 0;

            int CtRankCalc = 0;
            int TRankCalc = 0;

            foreach (Entity Player in Game.EntityList)
            {
                int PlayerResource = Memory.ReadMemory<int>(Memory.offsets.signatures.dwPlayerResource);
                int Wins = Memory.ReadMemory<int>(PlayerResource +  Memory.offsets.netvars.m_iCompetitiveWins + ((Player.EntityBase + 1) * 4)); 
                int Rank = Memory.ReadMemory<int>(PlayerResource +  Memory.offsets.netvars.m_iCompetitiveRanking + ((Player.EntityBase + 1) * 4));

                string RankName = Utility.GetRankNameFromNumber(Rank);

              
                    dataGridView1.Invoke(new Action(() => { 
                        dataGridView1.Rows.Add(Player.GetTeamName(), Player.GetName(), (Player.FlashDuration) != 0, RankName, Wins);
                        dataGridView1.Sort(dataGridView1.Columns["Flashed"], ListSortDirection.Descending);
                    }));
                

                
                if (Player.Team == 2)
                {
                    TotalTs++;
                    TRankCalc = TRankCalc + Rank;
                }
                if (Player.Team == 3)
                {
                    TotalCTs++;
                    CtRankCalc = CtRankCalc + Rank;
                }
            }


            int TAverage = 0;
            int CTAverage = 0;
            if (TotalTs != 0 && TRankCalc != 0)
            {
                TAverage = TRankCalc / TotalTs;
            }

            if (TotalCTs != 0 && CtRankCalc != 0)
            {
                CTAverage = CtRankCalc / TotalCTs;
            }

            CTRankAvLabel.Invoke(new Action(() => { CTRankAvLabel.Text = "CT Rank Average:" + Utility.GetRankNameFromNumber(CTAverage); }));
            TRankAvLabel.Invoke(new Action(() => { TRankAvLabel.Text = "T Rank Average: " + Utility.GetRankNameFromNumber(TAverage); }));
            dataGridView1.Invoke(new Action(() => { dataGridView1.AutoResizeColumns(); }));
        }

        private void RefreshButton_Click(object sender, EventArgs e)
        {
            updateDataGrid();
            autoUpdate = !autoUpdate;
        }
    }
}
